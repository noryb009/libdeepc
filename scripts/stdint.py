#!/usr/bin/env python

import os

from subprocess import run

from typing import Any, Dict, List, Optional

CC = ['gcc', '-o']
CC_SIZE_OF = CC + ['size-of', 'size-of.c']
CC_IS_CHAR_SIGNED = CC + ['is-char-signed', 'is-char-signed.c']

TYPES = ['int', 'char', 'short', 'long', 'long long']
FLOAT_TYPES = ['float', 'double', 'long double']
# Unsigned or signed only types.
SIGNED_ONLY_TYPES = []
UNSIGNED_ONLY_TYPES = []

SIGNED_TYPES   = [  'signed {}'.format(t) for t in TYPES] +   SIGNED_ONLY_TYPES
UNSIGNED_TYPES = ['unsigned {}'.format(t) for t in TYPES] + UNSIGNED_ONLY_TYPES
ALL_TYPES = SIGNED_TYPES + UNSIGNED_TYPES + FLOAT_TYPES

# Number of bits in a char (byte). 8 is the only "real" supported value.
CHAR_BIT = 8

REQUIRED_SIZES = [8, 16, 32, 64]
MAX_REQUIRED = max(REQUIRED_SIZES)

# Smallest types that are at least n bytes. We use a map here in case
# we want to adjust this, since later code needs the actual sizes.
LEAST_SIZES = {8: 8, 16: 16, 32: 32, 64: 64}
# Fastest type that is at least n bytes. We just assume the regular
# type is the fastest. Note that my version of GCC uses 1, 8, 8, 8.
FAST_SIZES = LEAST_SIZES

# Non-char types that are defined in `limits.h`.
LIMITS = [('SHRT', 'short'), ('INT', 'int'), ('LONG', 'long'), ('LLONG', 'long long')]

with open('size-of.c.in', 'r') as f:
  SIZE_OF_C_IN = f.read()

def literal_suffix(t: str, signed: bool) -> str:
  suffix = ''
  if 'long long' in t:
    suffix = 'LL'
  elif 'long' in t:
    suffix = 'L'
  sign = '' if signed else 'U'
  return '{}{}'.format(sign, suffix)

def size_of(t: str) -> Optional[int]:
  size_of_c = '''
  #define T {}
  {}
  '''
  with open('size-of.c', 'w+') as f:
    f.write(size_of_c.format(t, SIZE_OF_C_IN))
  if run(CC_SIZE_OF).returncode != 0:
    return None
  rc = run(['./size-of']).returncode * CHAR_BIT
  os.remove('./size-of')
  os.remove('./size-of.c')
  return rc


def is_char_signed() -> bool:
  assert run(CC_IS_CHAR_SIGNED).returncode == 0
  rc = run(['./is-char-signed']).returncode
  os.remove('./is-char-signed')
  return rc == 0


def make_best_types(types: List[str], type_sizes: Dict[str, int]) -> Dict[int, str]:
  best_types: Dict[int, str] = {}
  for t in types:
    if t in type_sizes:
      sz = type_sizes[t]
      if sz not in best_types:
        best_types[sz] = t
  return best_types


def make_stdint(
    type_sizes: Dict[str, int],
    best_types_s: Dict[int, str],
    best_types_u: Dict[int, str],
  ) -> None:
  ptr_size = size_of('void *')
  assert ptr_size in best_types_s
  assert ptr_size in best_types_u

  def header_f(f: Any) -> None:
    f.write('// DO NOT EDIT!\n')
    f.write('// THIS WAS GENERATED BY `stdint.py`\n\n')

    f.write('#pragma once\n\n')

  def define_f(f: Any, dst: str, src: str) -> None:
    f.write('#define {} {}\n'.format(dst, src))

  with open('../include/__stdint.h', 'w+') as f:
    def define(dst: str, src: str) -> None:
      define_f(f, dst, src)

    header_f(f)

    # `uint8_t` and friends.
    for sz in REQUIRED_SIZES:
      define(' __int{}_t'.format(sz), best_types_s[sz])
      define('__uint{}_t'.format(sz), best_types_u[sz])

    # `int_least8_t` and friends.
    for sz in REQUIRED_SIZES:
      define(' __int_least{}_t'.format(sz), ' __int{}_t'.format(LEAST_SIZES[sz]))
      define('__uint_least{}_t'.format(sz), '__uint{}_t'.format(LEAST_SIZES[sz]))
    for sz in REQUIRED_SIZES:
      define(' __int_fast{}_t'.format(sz), ' __int{}_t'.format(FAST_SIZES[sz]))
      define('__uint_fast{}_t'.format(sz), '__uint{}_t'.format(FAST_SIZES[sz]))

    # `intmax_t` and friends.
    max_int_size  = max(type_sizes[t] for t in   SIGNED_TYPES)
    umax_int_size = max(type_sizes[t] for t in UNSIGNED_TYPES)
    define(' __intmax_t', best_types_s[ max_int_size])
    define('__uintmax_t', best_types_u[umax_int_size])

    # `intptr_t` and friends.
    define(' __intptr_t', best_types_s[ptr_size])
    define('__uintptr_t', best_types_u[ptr_size])
    define('   __size_t', best_types_u[ptr_size])
    define('__ptrdiff_t', best_types_s[ptr_size])

    # For `max_align_t`, we want a largest type.
    # To make this more deterministic, we use the first lexographically.
    max_size = max(type_sizes.values())
    max_types = [k for k, v in type_sizes.items() if v == max_size]
    max_types.sort()
    define('__max_align_t', max_types[0])

    def define_c(name: str, t: str, signed: bool) -> None:
      suffix = literal_suffix(t, signed)
      if suffix != '':
        suffix = ' ## {}'.format(suffix)
      define('{}(c)'.format(name), 'c{}'.format(suffix))

    # `INT8_C` and friends.
    for sz in REQUIRED_SIZES:
      define_c(' __c_INT{}_C'.format(sz), best_types_s[sz], True)
      define_c('__c_UINT{}_C'.format(sz), best_types_u[sz], False)
    define_c(' __INTMAX_C'.format(sz), best_types_s[ max_int_size], True)
    define_c('__UINTMAX_C'.format(sz), best_types_u[umax_int_size], False)

    # `INT8_MAX` and friends.
    for sz in REQUIRED_SIZES:
      umax_val = 1 << sz
      max_val = umax_val // 2 - 1
      define(' __INT{}_MIN'.format(sz), '(-__c_INT{}_C({})-1)'.format(sz, max_val))
      define(' __INT{}_MAX'.format(sz), '  __c_INT{}_C({})'.format(sz, max_val))
      define('__UINT{}_MAX'.format(sz), ' __c_UINT{}_C({})'.format(sz, umax_val - 1))

    # `INT_LEAST8_MAX` and friends.
    for sz in REQUIRED_SIZES:
      actual_size = LEAST_SIZES[sz]
      define(' __INT_LEAST{}_MIN'.format(sz), ' __c_INT{}_MIN'.format(actual_size))
      define(' __INT_LEAST{}_MAX'.format(sz), ' __c_INT{}_MAX'.format(actual_size))
      define('__UINT_LEAST{}_MAX'.format(sz), '__c_UINT{}_MAX'.format(actual_size))
    for sz in REQUIRED_SIZES:
      actual_size = FAST_SIZES[sz]
      define(' __INT_FAST{}_MIN'.format(sz), ' __c_INT{}_MIN'.format(actual_size))
      define(' __INT_FAST{}_MAX'.format(sz), ' __c_INT{}_MAX'.format(actual_size))
      define('__UINT_FAST{}_MAX'.format(sz), '__c_UINT{}_MAX'.format(actual_size))

    # `INTMAX_MAX` and friends.
    define(' __INTMAX_MIN',  ' __INT{}_MIN'.format( max_int_size))
    define(' __INTMAX_MAX',  ' __INT{}_MAX'.format( max_int_size))
    define('__UINTMAX_MAX',  '__UINT{}_MAX'.format(umax_int_size))

    # `INTPTR_MAX` and friends.
    define(' __INTPTR_MIN',  ' __INT{}_MIN'.format(ptr_size))
    define(' __INTPTR_MAX',  ' __INT{}_MAX'.format(ptr_size))
    define('__UINTPTR_MAX',  '__UINT{}_MAX'.format(ptr_size))
    define(' __PTRDIFF_MIN', ' __INT{}_MIN'.format(ptr_size))
    define(' __PTRDIFF_MAX', ' __INT{}_MAX'.format(ptr_size))
    define('__SIZE_MAX',     '__UINT{}_MAX'.format(ptr_size))

  with open('../include/__inttypes.h', 'w+') as f:
    def define(dst: str, src: str) -> None:
      define_f(f, dst, src)

    header_f(f)

    def output_symbol(def_suffix: str, format_prefix: str) -> None:
      SYMBOLS = ['d', 'i', 'u', 'o', 'x', 'X']
      for symbol in SYMBOLS:
        s = '{}{}'.format(symbol, def_suffix)
        symbol_format = format_prefix + symbol
        define('PRI{}'.format(s), '"{}"'.format(symbol_format))
        define('SCN{}'.format(s), '"{}"'.format(symbol_format))

    def type_to_suffix(t: str) -> str:
      if 'long long' in t:
        return 'll'
      elif 'long' in t:
        return 'l'
      elif 'char' in t:
        return 'hh'
      elif 'short' in t:
        return 'h'
      return ''

    for sz in REQUIRED_SIZES:
      output_symbol(sz, type_to_suffix(best_types_u[sz]))
    for sz in REQUIRED_SIZES:
      output_symbol('LEAST{}'.format(sz), type_to_suffix(best_types_u[LEAST_SIZES[sz]]))
    for sz in REQUIRED_SIZES:
      output_symbol('FAST{}'.format(sz), type_to_suffix(best_types_u[FAST_SIZES[sz]]))
    output_symbol('MAX', 'j')
    output_symbol('PTR', 't')

  with open('../include/__limits.h', 'w+') as f:
    def define(dst: str, src: str) -> None:
      define_f(f, dst, src)

    header_f(f)

    f.write('#include <__stdint.h>\n\n')

    define('CHAR_BIT', '{}'.format(CHAR_BIT))

    char_max = 127 if is_char_signed() else 255
    char_min = char_max - 255
    define('CHAR_MIN', '({})'.format(char_min))
    define('CHAR_MAX', '({})'.format(char_max))

    define('SCHAR_MIN', ' __INT8_MIN')
    define('SCHAR_MAX', ' __INT8_MAX')
    define('UCHAR_MAX', '__UINT8_MAX')

    def minmax_or_number(
        t: str,
        signed: bool,
        is_max: bool,
      ) -> str:
      # unsigned minimum doesn't make sense.
      assert is_max or signed
      if signed:
        t = 'signed {}'.format(t)
      else:
        t = 'unsigned {}'.format(t)
      sz = type_sizes[t]
      if sz in REQUIRED_SIZES and False:
        sign_char = '' if signed else 'U'
        minmax = 'MAX' if is_max else 'MIN'
        return '__{}INT{}_{}'.format(sign_char, sz, minmax)

      suffix = literal_suffix(t, signed)
      umax_val = 1 << sz
      max_val = umax_val // 2 - 1
      if not signed:
        return '{}{}'.format(umax_val - 1, suffix)
      if is_max:
        return '{}{}'.format(max_val, suffix)
      return '(-{}{}-1)'.format(max_val, suffix)

    for (name, t) in LIMITS:
      define(' {}_MIN'.format(name), minmax_or_number(t, signed=True, is_max=False))
      define(' {}_MAX'.format(name), minmax_or_number(t, signed=True, is_max=True))
      define('U{}_MAX'.format(name), minmax_or_number(t, signed=False, is_max=True))

if __name__ == '__main__':
  type_sizes: Dict[str, int] = {}
  for t in ALL_TYPES:
    sz = size_of(t)
    if sz is not None:
      type_sizes[t] = sz

  # Find the "best" type for each size. Eg. if `int` and `long` are both
  # 32 bits, using `int` seems more natual. This also ensures floating point
  # types aren't used as ints.
  best_types_s = make_best_types(  SIGNED_TYPES, type_sizes)
  best_types_u = make_best_types(UNSIGNED_TYPES, type_sizes)

  # Make sure we have at least one type for all needed types.
  for sz in REQUIRED_SIZES:
    assert best_types_s[sz] is not None
    assert best_types_u[sz] is not None

  # Create the `__stdint.h` header.
  make_stdint(type_sizes, best_types_s, best_types_u)

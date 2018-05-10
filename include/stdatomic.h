#pragma once

#include <stddef.h> // TODO: Is this OK?
#include <stdint.h>

typedef enum {
  memory_order_relaxed,
  memory_order_consume,
  memory_order_acquire,
  memory_order_release,
  memory_order_acq_rel,
  memory_order_seq_cst,
} memory_order;

typedef _Atomic _Bool               atomic_bool;
typedef _Atomic char                atomic_char;
typedef _Atomic signed char         atomic_schar;
typedef _Atomic unsigned char       atomic_uchar;
typedef _Atomic short               atomic_short;
typedef _Atomic unsigned short      atomic_ushort;
typedef _Atomic int                 atomic_int;
typedef _Atomic unsigned int        atomic_uint;
typedef _Atomic long                atomic_long;
typedef _Atomic unsigned long       atomic_ulong;
typedef _Atomic long long           atomic_llong;
typedef _Atomic unsigned long long  atomic_ullong;
/*
typedef _Atomic char16_t            atomic_char16_t;
typedef _Atomic char32_t            atomic_char32_t;
typedef _Atomic wchar_t             atomic_wchar_t;
*/
typedef _Atomic int_least8_t        atomic_int_least8_t;
typedef _Atomic uint_least8_t       atomic_uint_least8_t;
typedef _Atomic int_least16_t       atomic_int_least16_t;
typedef _Atomic uint_least16_t      atomic_uint_least16_t;
typedef _Atomic int_least32_t       atomic_int_least32_t;
typedef _Atomic uint_least32_t      atomic_uint_least32_t;
typedef _Atomic int_least64_t       atomic_int_least64_t;
typedef _Atomic uint_least64_t      atomic_uint_least64_t;
typedef _Atomic int_fast8_t         atomic_int_fast8_t;
typedef _Atomic uint_fast8_t        atomic_uint_fast8_t;
typedef _Atomic int_fast16_t        atomic_int_fast16_t;
typedef _Atomic uint_fast16_t       atomic_uint_fast16_t;
typedef _Atomic int_fast32_t        atomic_int_fast32_t;
typedef _Atomic uint_fast32_t       atomic_uint_fast32_t;
typedef _Atomic int_fast64_t        atomic_int_fast64_t;
typedef _Atomic uint_fast64_t       atomic_uint_fast64_t;
typedef _Atomic intptr_t            atomic_intptr_t;
typedef _Atomic uintptr_t           atomic_uintptr_t;
typedef _Atomic size_t              atomic_size_t;
typedef _Atomic ptrdiff_t           atomic_ptrdiff_t;
typedef _Atomic intmax_t            atomic_intmax_t;
typedef _Atomic uintmax_t           atomic_uintmax_t;

#define ATOMIC_FLAG_INIT (0)
#define ATOMIC_VAR_INIT(value) {(value)}

typedef __atomic_flag atomic_flag;

_Bool atomic_flag_test_and_set(volatile atomic_flag *flag);
_Bool atomic_flag_test_and_set_explicit(volatile atomic_flag *flag, memory_order order);

void atomic_flag_clear(volatile atomic_flag *flag);
void atomic_flag_clear_explicit(volatile atomic_flag *flag, memory_order order);

// TODO: Other atomic operations.

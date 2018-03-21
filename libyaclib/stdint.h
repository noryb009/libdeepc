#pragma once

#include "stdinternaltypes.h"

// NOTE: This is GCC-on-x86-64 specific.
// TODO: #if's for choosing the correct size.
typedef signed char    int8_t;
typedef signed short   int16_t;
typedef signed int     int32_t;
typedef signed long    int64_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;

// Smallest types that are at least n bytes.
typedef int8_t   int_least8_t;
typedef int16_t  int_least16_t;
typedef int32_t  int_least32_t;
typedef int64_t  int_least64_t;
typedef uint8_t  uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

// Fastest type that is at least n bytes. We just assume the regular type is
// the fastest. Note that my version of GCC uses 1, 8, 8, 8.
typedef int8_t   int_fast8_t;
typedef int16_t  int_fast16_t;
typedef int32_t  int_fast32_t;
typedef int64_t  int_fast64_t;
typedef uint8_t  uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

// We assume 64 bits.
typedef __intptr_t  intptr_t;
typedef __uintptr_t uintptr_t;

// We don't really care about anything bigger.
typedef int64_t  intmax_t;
typedef uint64_t uintmax_t;

#define INT8_MAX 127
#define INT8_MIN -128
#define UINT8_MAX 255

#define INT16_MAX 32767
#define INT16_MIN -32768
#define UINT16_MAX 65535

#define INT32_MAX 2147483647
#define INT32_MIN -2147483648
#define UINT32_MAX 4294967295

#define INT64_MAX 9223372036854775807l
#define INT64_MIN -9223372036854775808l
#define UINT64_MAX 18446744073709551615ul

#define INT8_LEAST_MIN INT8_MIN
#define INT8_LEAST_MAX INT8_MAX
#define UINT8_LEAST_MAX UINT8_MAX

#define INT16_LEAST_MIN INT16_MIN
#define INT16_LEAST_MAX INT16_MAX
#define UINT16_LEAST_MAX UINT16_MAX

#define INT32_LEAST_MIN INT32_MIN
#define INT32_LEAST_MAX INT32_MAX
#define UINT32_LEAST_MAX UINT32_MAX

#define INT64_LEAST_MIN INT64_MIN
#define INT64_LEAST_MAX INT64_MAX
#define UINT64_LEAST_MAX UINT64_MAX

#define INT8_FAST_MIN INT8_MIN
#define INT8_FAST_MAX INT8_MAX
#define UINT8_FAST_MAX UINT8_MAX

#define INT16_FAST_MIN INT16_MIN
#define INT16_FAST_MAX INT16_MAX
#define UINT16_FAST_MAX UINT16_MAX

#define INT32_FAST_MIN INT32_MIN
#define INT32_FAST_MAX INT32_MAX
#define UINT32_FAST_MAX UINT32_MAX

#define INT64_FAST_MIN INT64_MIN
#define INT64_FAST_MAX INT64_MAX
#define UINT64_FAST_MAX UINT64_MAX

#define INTPTR_MIN INT64_MIN
#define INTPTR_MAX INT64_MAX
#define UINTPTR_MAX UINT64_MAX

#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#define INT8_C  int_least8_t
#define INT16_C int_least16_t
#define INT32_C int_least32_t
#define INT64_C int_least64_t

#define UINT8_C  uint_least8_t
#define UINT16_C uint_least16_t
#define UINT32_C uint_least32_t
#define UINT64_C uint_least64_t

#define INTMAX_C  intmax_t
#define UINTMAX_C uintmax_t

#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX

#define SIZE_MAX UINTPTR_MAX

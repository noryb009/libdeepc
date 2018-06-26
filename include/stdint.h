#pragma once

#include <__limits.h>
#include <__stdint.h>

typedef   __int8_t   int8_t;
typedef  __uint8_t  uint8_t;
typedef  __int16_t  int16_t;
typedef __uint16_t uint16_t;
typedef  __int32_t  int32_t;
typedef __uint32_t uint32_t;
typedef  __int64_t  int64_t;
typedef __uint64_t uint64_t;

typedef  __int_least8_t   int_least8_t;
typedef __uint_least8_t  uint_least8_t;
typedef  __int_least16_t  int_least16_t;
typedef __uint_least16_t uint_least16_t;
typedef  __int_least32_t  int_least32_t;
typedef __uint_least32_t uint_least32_t;
typedef  __int_least64_t  int_least64_t;
typedef __uint_least64_t uint_least64_t;

typedef  __int_fast8_t   int_fast8_t;
typedef __uint_fast8_t  uint_fast8_t;
typedef  __int_fast16_t  int_fast16_t;
typedef __uint_fast16_t uint_fast16_t;
typedef  __int_fast32_t  int_fast32_t;
typedef __uint_fast32_t uint_fast32_t;
typedef  __int_fast64_t  int_fast64_t;
typedef __uint_fast64_t uint_fast64_t;

typedef __intptr_t  intptr_t;
typedef __uintptr_t uintptr_t;

typedef __intmax_t  intmax_t;
typedef __uintmax_t uintmax_t;

#define  INT8_MIN  __INT8_MIN
#define  INT8_MAX  __INT8_MAX
#define UINT8_MAX __UINT8_MAX

#define  INT16_MIN  __INT16_MIN
#define  INT16_MAX  __INT16_MAX
#define UINT16_MAX __UINT16_MAX

#define  INT32_MIN  __INT32_MIN
#define  INT32_MAX  __INT32_MAX
#define UINT32_MAX __UINT32_MAX

#define  INT64_MIN  __INT64_MIN
#define  INT64_MAX  __INT64_MAX
#define UINT64_MAX __UINT64_MAX

#define  INT8_LEAST_MIN  __INT8_LEAST_MIN
#define  INT8_LEAST_MAX  __INT8_LEAST_MAX
#define UINT8_LEAST_MAX __UINT8_LEAST_MAX

#define  INT16_LEAST_MIN  __INT16_LEAST_MIN
#define  INT16_LEAST_MAX  __INT16_LEAST_MAX
#define UINT16_LEAST_MAX __UINT16_LEAST_MAX

#define  INT32_LEAST_MIN  __INT32_LEAST_MIN
#define  INT32_LEAST_MAX  __INT32_LEAST_MAX
#define UINT32_LEAST_MAX __UINT32_LEAST_MAX

#define  INT64_LEAST_MIN  __INT64_LEAST_MIN
#define  INT64_LEAST_MAX  __INT64_LEAST_MAX
#define UINT64_LEAST_MAX __UINT64_LEAST_MAX

#define  INT8_FAST_MIN  __INT8_FAST_MIN
#define  INT8_FAST_MAX  __INT8_FAST_MAX
#define UINT8_FAST_MAX __UINT8_FAST_MAX

#define  INT16_FAST_MIN  __INT16_FAST_MIN
#define  INT16_FAST_MAX  __INT16_FAST_MAX
#define UINT16_FAST_MAX __UINT16_FAST_MAX

#define  INT32_FAST_MIN  __INT32_FAST_MIN
#define  INT32_FAST_MAX  __INT32_FAST_MAX
#define UINT32_FAST_MAX __UINT32_FAST_MAX

#define  INT64_FAST_MIN  __INT64_FAST_MIN
#define  INT64_FAST_MAX  __INT64_FAST_MAX
#define UINT64_FAST_MAX __UINT64_FAST_MAX

#define  INTMAX_MIN  __INTMAX_MIN
#define  INTMAX_MAX  __INTMAX_MAX
#define UINTMAX_MAX __UINTMAX_MAX

#define INTPTR_MIN  __INTPTR_MIN
#define INTPTR_MAX  __INTPTR_MAX
#define UINTPTR_MAX __UINTPTR_MAX

#define PTRDIFF_MIN __PTRDIFF_MIN
#define PTRDIFF_MAX __PTRDIFF_MAX
#define SIZE_MAX __SIZE_MAX

#define  INT8_C(c)   __c_INT8_C(c)
#define UINT8_C(c)  __c_UINT8_C(c)
#define  INT16_C(c)  __c_INT16_C(c)
#define UINT16_C(c) __c_UINT16_C(c)
#define  INT32_C(c)  __c_INT32_C(c)
#define UINT32_C(c) __c_UINT32_C(c)
#define  INT64_C(c)  __c_INT64_C(c)
#define UINT64_C(c) __c_UINT64_C(c)

#define  INTMAX_C(c)  __INTMAX_C(c)
#define UINTMAX_C(c) __UINTMAX_C(c)

#define SIG_ATOMIC_MIN __INT_MIN
#define SIG_ATOMIC_MAX __INT_MAX

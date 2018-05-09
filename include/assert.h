#pragma once

#include <stdinternaltypes.h>

_Noreturn void __assert_failure(const char *file, __size_t line, const char *func);

#ifdef NDEBUG
#define assert(cond) do {} while(0)
#else
#define assert(cond) \
  do { \
    if (!(cond)) { \
      __assert_failure(__FILE__, __LINE__, __func__); \
    } \
  } while (0)
#endif

#define static_assert _Static_assert

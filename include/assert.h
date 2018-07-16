// POSIX requires assert to be redefined according to NDEBUG whenever
// this file is included, so this file doesn't have a header guard.

#include <gen/stdint.h>

#undef assert

_Noreturn void __assert_failure(const char *cond, const char *file, __size_t line, const char *func);

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
#define assert(cond) \
  do { \
    if (!(cond)) { \
      __assert_failure(#cond, __FILE__, __LINE__, __func__); \
    } \
  } while (0)
#endif

// There's no point in making a header guard for this...
#ifndef static_assert
# define static_assert _Static_assert
#endif

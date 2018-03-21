#pragma once

#include "stdinternaltypes.h"

extern _Noreturn void __abort();

// TODO: Output something useful.
#ifdef NDEBUG
#define assert(cond) do {} while(0)
#else
#define assert(cond) \
  do { \
    if (!(cond)) { \
      __abort(); \
    } \
  } while (0)
#endif

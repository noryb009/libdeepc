#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

_Noreturn void __assert_failure(const char *file, size_t line, const char *func) {
  // TODO: Output to stderr, not stdout.
  // TODO: Unsigned line number.
  printf("Assert failure:\n  File: %s\n  Func: %s\n  Line: %d\n", file, func, (int)line);
  abort();
}

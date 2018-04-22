#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

_Noreturn void __assert_failure(const char *file, size_t line, const char *func) {
  // TODO: Output to stderr, not stdout.
  // TODO: Output line number.
  printf("Assert failure:\n  File: %s\n  Func: %s\n", file, func);
  abort();
}

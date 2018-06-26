#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

_Noreturn void __assert_failure(const char *cond, const char *file, size_t line, const char *func) {
  fprintf(stderr, "Assert failed:\n  Cond: %s\n  File: %s:%zu\n  Func: %s\n", cond, file, line, func);
  abort();
}

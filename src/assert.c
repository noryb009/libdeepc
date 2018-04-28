#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

_Noreturn void __assert_failure(const char *file, size_t line, const char *func) {
  fprintf(stderr, "Assert failure:\n  File: %s\n  Func: %s\n  Line: %zu\n", file, func, line);
  abort();
}

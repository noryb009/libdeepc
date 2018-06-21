#include <stdarg.h>
#include <fcntl.h>

#include "syscall.h"

int open(const char *pathname, int flags, ...) {
  mode_t mode = 0;
  if (flags & (O_CREAT|O_TMPFILE)) {
    // We need to get the mode.
    va_list args;
    va_start(args, flags);
    mode = va_arg(args, mode_t);
    // TODO: Do we need to do anything with umask?
    va_end(args);
  }

  return __syscall3((uintptr_t)pathname, flags, mode, SYS_OPEN);
}

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "syscall.h"

void *sbrk(intptr_t increment) {
  const uint64_t cur_break = __syscall1(increment, SYS_BRK);
  if (increment == 0) {
    return (void *)cur_break;
  }
  const uint64_t next_break = cur_break + increment;
  const uint64_t actual_next_break = __syscall1(next_break, SYS_BRK);
  if (next_break != actual_next_break) {
    errno = ENOMEM;
    return (void *)-1;
  }
  return (void *)cur_break;
}

ssize_t read(int fd, void *buf, size_t count) {
  return (ssize_t)__syscall3(fd, (uint64_t)buf, count, SYS_READ);
}

ssize_t write(int fd, const void *buf, size_t count) {
  return (ssize_t)__syscall3(fd, (uint64_t)buf, count, SYS_WRITE);
}

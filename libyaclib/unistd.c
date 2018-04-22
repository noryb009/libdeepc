#include "errno.h"
#include "stddef.h"
#include "stdint.h"
#include "unistd.h"

typedef enum {
  SYS_READ = 0,
  SYS_WRITE = 1,
  SYS_BRK = 12,
} Syscall;

extern uint64_t __syscall0(Syscall);
extern uint64_t __syscall1(uint64_t, Syscall);
extern uint64_t __syscall2(uint64_t, uint64_t, Syscall);
extern uint64_t __syscall3(uint64_t, uint64_t, uint64_t, Syscall);
extern uint64_t __syscall4(uint64_t, uint64_t, uint64_t, uint64_t, Syscall);
extern uint64_t __syscall5(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, Syscall);

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
  const ssize_t ret = (ssize_t)__syscall3(fd, (uint64_t)buf, count, SYS_READ);
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return ret;
}

ssize_t write(int fd, const void *buf, size_t count) {
  const ssize_t ret = (ssize_t)__syscall3(fd, (uint64_t)buf, count, SYS_WRITE);
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return ret;
}

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall.h"

void *sbrk(intptr_t increment) {
  const uint64_t cur_break = __syscall1(0, SYS_BRK);
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

int close(int fd) {
  return (int)__syscall1(fd, SYS_CLOSE);
}

ssize_t write(int fd, const void *buf, size_t count) {
  return (ssize_t)__syscall3(fd, (uint64_t)buf, count, SYS_WRITE);
}

pid_t getpid(void) {
  return __syscall0(SYS_GETPID);
}

pid_t fork(void) {
  return __syscall0(SYS_FORK);
}

_Noreturn void execve(
      const char *filename,
      const char *const *argv,
      char *const envp[]) {
  __syscall3_noret((uintptr_t)filename, (uintptr_t)argv, (uintptr_t)envp, SYS_EXECVE);
}

pid_t waitpid(pid_t pid, int *stat_loc, int options) {
  return __syscall4(pid, (uintptr_t)stat_loc, options, (uintptr_t)NULL, SYS_WAIT4);
}

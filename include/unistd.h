#pragma once

#include <stddef.h>
#include <stdint.h>

typedef signed long ssize_t;
typedef int pid_t;
typedef int uid_t;

// TODO: Type declarations.

void *sbrk(intptr_t increment);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int close(int fd);
pid_t getpid(void);
pid_t fork(void);
_Noreturn void execve(
      const char *filename,
      const char *const *argv,
      char *const envp[]);

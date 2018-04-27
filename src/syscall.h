#pragma once

#include <stdint.h>

typedef enum {
  SYS_READ = 0,
  SYS_WRITE = 1,
  SYS_BRK = 12,
  SYS_EXIT = 60,
} Syscall;

extern uint64_t __syscall0(Syscall);
extern uint64_t __syscall1(uint64_t, Syscall);
extern uint64_t __syscall2(uint64_t, uint64_t, Syscall);
extern uint64_t __syscall3(uint64_t, uint64_t, uint64_t, Syscall);
extern uint64_t __syscall4(uint64_t, uint64_t, uint64_t, uint64_t, Syscall);
extern uint64_t __syscall5(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, Syscall);

extern _Noreturn void __syscall1_noret(uint64_t, Syscall);

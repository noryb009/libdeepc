#pragma once

#include <stdint.h>

typedef enum {
  SYS_READ = 0,
  SYS_WRITE = 1,
  SYS_BRK = 12,
  SYS_CLONE = 56,
  SYS_EXIT = 60,
  SYS_ARCH_PRCTL = 158,
} Syscall;

#define CLONE_VM             0x00000100
#define CLONE_FS             0x00000200
#define CLONE_FILES          0x00000400
#define CLONE_SIGHAND        0x00000800
#define CLONE_PARENT         0x00008000
#define CLONE_THREAD         0x00010000
#define CLONE_PARENT_SETTID  0x00100000

// For arch_prctl.
#define ARCH_SET_GS 0x1001
#define ARCH_SET_FS 0x1002
#define ARCH_GET_FS 0x1003
#define ARCH_GET_GS 0x1004

extern uint64_t __syscall0(Syscall);
extern uint64_t __syscall1(uint64_t, Syscall);
extern uint64_t __syscall2(uint64_t, uint64_t, Syscall);
extern uint64_t __syscall3(uint64_t, uint64_t, uint64_t, Syscall);
extern uint64_t __syscall4(uint64_t, uint64_t, uint64_t, uint64_t, Syscall);
extern uint64_t __syscall5(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, Syscall);

extern _Noreturn void __syscall1_noret(uint64_t, Syscall);

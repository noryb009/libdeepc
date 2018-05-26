#include "errno.h"
#include "syscall.h"

static int64_t syscall_chk_err(int64_t ret) {
  if ((uint64_t)ret > (uint64_t)-4096) {
    errno = -ret;
    return -1;
  }
  return ret;
}

int64_t __syscall0(Syscall s) {
  return syscall_chk_err(__syscall_raw0(s));
}
int64_t __syscall1(uint64_t a, Syscall s) {
  return syscall_chk_err(__syscall_raw1(a, s));
}
int64_t __syscall2(uint64_t a, uint64_t b, Syscall s) {
  return syscall_chk_err(__syscall_raw2(a, b, s));
}
int64_t __syscall3(uint64_t a, uint64_t b, uint64_t c, Syscall s) {
  return syscall_chk_err(__syscall_raw3(a, b, c, s));
}
int64_t __syscall4(uint64_t a, uint64_t b, uint64_t c, uint64_t d, Syscall s) {
  return syscall_chk_err(__syscall_raw4(a, b, c, d, s));
}
int64_t __syscall5(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, Syscall s) {
  return syscall_chk_err(__syscall_raw5(a, b, c, d, e, s));
}

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "syscall.h"

double difftime(time_t time_end, time_t time_begin) {
  return time_end - time_begin;
}

static bool get_clock(struct timespec *ts, uint64_t mode) {
  // TODO: Use vdso.
  const uint64_t ret = __syscall2(mode, (uint64_t)ts, SYS_CLOCK_GETTIME) > -4096UL;
  if (ret > -4096UL) {
    errno = -ret;
    return false;
  }
  return true;
}

time_t time(time_t *arg) {
  struct timespec ts;
  if (!get_clock(&ts, CLOCK_REALTIME)) {
    if (arg) {
      *arg = (time_t)-1;
    }
    return (time_t)-1;
  }
  if (arg) {
    *arg = ts.tv_sec;
  }
  return ts.tv_sec;
}

clock_t clock(void) {
  struct timespec ts;
  if (!get_clock(&ts, CLOCK_PROCESS_CPUTIME_ID)) {
    return (clock_t)-1;
  }
  clock_t ret;
  ret = ts.tv_sec * CLOCKS_PER_SEC;
  const long nanosec_in_sec = 1000000000;
  ret += ts.tv_nsec / (nanosec_in_sec / CLOCKS_PER_SEC);
  return ret;
}

int timespec_get(struct timespec *ts, int base) {
  // TODO: What other bases are useful?
  assert(base == TIME_UTC);

  if (!get_clock(ts, CLOCK_REALTIME)) {
    return 0;
  }
  return base;
}

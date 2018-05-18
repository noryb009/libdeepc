#pragma once

#define CLOCKS_PER_SEC 1000000
#define TIME_UTC -1

typedef long time_t;
typedef long clock_t;

enum {
  CLOCK_REALTIME = 0,
  CLOCK_MONOTONIC = 1,
  CLOCK_PROCESS_CPUTIME_ID = 2,
  CLOCK_THREAD_CPUTIME_ID =	3,
};

struct timespec {
  time_t tv_sec;
  long   tv_nsec;
};

double difftime(time_t time_end, time_t time_begin);
time_t time(time_t *arg);
clock_t clock(void);
int timespec_get(struct timespec *ts, int base);

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

//#define OUTPUT 1

int main(void) {
  const clock_t c1 = clock();
  {
    time_t t;
    const time_t r1 = time(&t);
    assert(t == r1);
    const time_t r2 = time(NULL);
    assert(r2 != -1);
#ifdef OUTPUT
    printf("time_t r1: %ld\n", r1);
    printf("time_t r2: %ld\n", r2);
    printf(" - delta: %ld\n", (long)difftime(r2, r1));
#endif
  }
  const clock_t c2 = clock();
  assert(c1 <= c2);
#ifdef OUTPUT
  printf("clock_t c1: %ld\n", c1);
  printf("clock_t c2: %ld\n", c2);
  printf(" - delta: %ld\n", c2 - c1);
#endif

  struct timespec ts[2];
  for (int i = 0; i < 2; i++) {
    assert(timespec_get(ts + i, TIME_UTC) != 0);
#ifdef OUTPUT
    printf("ts  sec: %ld\n", ts[i].tv_sec);
    printf("ts nsec: %ld\n", ts[i].tv_nsec);
#endif
  }
  long d1, d2;
  d1 = ts[1].tv_sec - ts[0].tv_sec;
  d2 = ts[1].tv_nsec - ts[0].tv_nsec;

  if (ts[0].tv_nsec > ts[1].tv_nsec) {
    // We have an extra sec and a negative nsec.
    d1--;
    d2 += 1000000000;
  }
#ifdef OUTPUT
  printf(" - delta  sec: %ld\n", d1);
  printf(" - delta nsec: %ld\n", d2);
#else
  (void)d1;
  (void)d2;
#endif
}

#include <assert.h>
#include <stdio.h>
#include <threads.h>

thread_local int a, b, c, d, e;

static int fn(void *arg) {
  a = 0;
  b = 0;
  c = 0;
  d = 0;
  e = 0;

  for (int i = 0; i < *(int *)arg; ++i) {
    a++;
    b -= a;
    c += b;
    d -= c;
    e += d;
  }

  return 0;
}

int main(void) {
#define THRDS 10
  thrd_t tn[THRDS];

  int arg = 100000;
  for (int i = 0; i < THRDS; ++i) {
    thrd_create(tn + i, fn, &arg);
  }

  for (int i = 0; i < THRDS; ++i) {
    thrd_join(tn[i], NULL);
  }
}

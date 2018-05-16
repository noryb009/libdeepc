#include <assert.h>
#include <stdio.h>
#include <threads.h>

typedef struct {
  volatile int *a;
  volatile int count;
  mtx_t mtx;
} info;

static int adder(void *arg) {
  info *inf = (info *)arg;
  volatile int *a = inf->a;

  for (int i = 0; i < inf->count; ++i) {
    mtx_lock(&inf->mtx);
    int b = *a;
    if (b == -1) {
      *a = b + 3;
    } else if (b == -2) {
      *a = b + 2;
    } else {
      *a = b + 1;
    }
    mtx_unlock(&inf->mtx);
  }
  return 2;
}

int main(void) {
  info inf;
  volatile int a = 0;

  inf.a = &a;
  inf.count = 100000;
  mtx_init(&inf.mtx, mtx_plain);

#define THRDS 10
  thrd_t tn[THRDS];

  for (int i = 0; i < THRDS; ++i) {
    thrd_create(tn + i, adder, &inf);
  }

  for (int i = 0; i < THRDS; ++i) {
    int k = 0;
    if (i == 3) {
      thrd_join(tn[i], &k);
      assert(k == 2);
    } else {
      thrd_join(tn[i], NULL);
    }
  }

  printf("Sum is: %d\n", a);

  mtx_destroy(&inf.mtx);
}

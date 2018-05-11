#include <assert.h>
#include <stdio.h>
#include <threads.h>

mtx_t mtx;

static int run(void *arg) {
  const int cnt = *(int *)arg;
  mtx_lock(&mtx);
  for (int i = 0; i < cnt; ++i) {
    printf("Running, val: %d\n", cnt);
  }
  mtx_unlock(&mtx);

  // TODO: Once thrd_exit work, enable this:
  //thrd_exit(-cnt);
  return -cnt;
}

int main(void) {
#define THRDS 5
  int nums[THRDS];
  for (int i = 0; i < THRDS; ++i) {
    nums[i] = i + 1;
  }

  mtx_init(&mtx, mtx_plain);
  thrd_t tn[THRDS];
  for (int i = 0; i < THRDS; ++i) {
    thrd_create(tn + i, run, nums + i);
  }

  for (int i = 0; i < THRDS; ++i) {
    int k;
    thrd_join(tn[i], &k);
    assert(k == -i - 1);
  }

  mtx_destroy(&mtx);
}
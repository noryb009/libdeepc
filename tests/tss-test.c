#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>

volatile bool p9_created = false;
volatile bool done_init = false;

tss_t p1;
tss_t counter;
tss_t p4;
tss_t p9;
mtx_t mtx;

static void count_down_dtor(void *p) {
  (*(int *)p)--;
}

int total_count = 0;
static void total_count_dtor(void *p) {
  (void)p;
  mtx_lock(&mtx);
  total_count++;
  mtx_unlock(&mtx);
}

static int run(void *p) {
  volatile int *v = (int *)p;
  const int position = (uintptr_t)p / sizeof(int) % 4;
  if (position < 2) {
    assert(tss_get(counter) == NULL);
  }
  tss_set(counter, p);
  (*v)++;
  assert(*v == 1);
  thrd_yield();
  assert(*v == 1);
  if (position == 0 || position == 3) {
    assert(tss_get(counter) == p);
  }

  done_init = true;
  while (!p9_created) {
    thrd_yield();
  }
  assert(tss_get(p9) == NULL);
  tss_set(p9, p);
  return 0;
}

int main(void) {
  mtx_init(&mtx, mtx_plain);

  tss_t spare;
  tss_create(&p1, NULL);
  tss_set(p1, (void *)111);
  tss_create(&spare, NULL);
  tss_create(&counter, count_down_dtor);
  tss_set(counter, (void *)123);
  tss_create(&p4, total_count_dtor);
  tss_set(p4, (void *)574);
  for (int i = 5; i < 7; ++i) {
    tss_create(&spare, NULL);
  }

#define THRDS 12
  thrd_t tn[THRDS];
  volatile int nums[THRDS] = {0};
  for (int i = 0; i < THRDS; ++i) {
    thrd_create(tn + i, run, (void *)(nums + i));
  }

  while (!done_init) {
    thrd_yield();
  }

  for (int i = 7; i < 9; ++i) {
    tss_create(&spare, NULL);
  }
  tss_create(&p9, total_count_dtor);
  tss_set(p9, (void *)579);
  p9_created = true;

  for (int i = 0; i < THRDS; ++i) {
    thrd_join(tn[i], NULL);
    assert(nums[i] == 0);
  }
  for (int i = 0; i < THRDS; ++i) {
    assert(nums[i] == 0);
  }
  total_count = -(THRDS + (THRDS / 2));

  assert((uintptr_t)tss_get(p1) == 111);
  assert((uintptr_t)tss_get(counter) == 123);
  assert((uintptr_t)tss_get(p4) == 574);
  assert((uintptr_t)tss_get(p9) == 579);

  tss_delete(p9);
  
  mtx_destroy(&mtx);
}

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "syscall.h"

#define AT_EXIT_FUNCS 32

static int cur_at_exit_func = 0;
static int cur_at_quick_exit_func = 0;
static __atexit_func (*at_exit_funcs[AT_EXIT_FUNCS]);
static __atexit_func (*at_quick_exit_funcs[AT_EXIT_FUNCS]);

int atexit(__atexit_func func) {
  // TODO: Make thread safe.
  if (cur_at_exit_func == AT_EXIT_FUNCS) {
    return 1;
  }
  at_exit_funcs[cur_at_exit_func++] = func;
  return 0;
}

int at_quick_exit(__atexit_func func) {
  // TODO: Make thread safe.
  if (cur_at_quick_exit_func == AT_EXIT_FUNCS) {
    return 1;
  }
  at_quick_exit_funcs[cur_at_quick_exit_func++] = func;
  return 0;
}

static void run_exit_funcs(__atexit_func (**fns), int entries) {
  for (int i = entries; i-- > 0;) {
    fns[i]();
  }
}

_Noreturn void exit(int ret) {
  run_exit_funcs(at_exit_funcs, cur_at_exit_func);
  // TODO: Flush and close C streams.
  // TODO: Remove files created by tmpfile.
  _Exit(ret);
}
_Noreturn void quick_exit(int ret) {
  run_exit_funcs(at_quick_exit_funcs, cur_at_quick_exit_func);
  exit(ret);
}

_Noreturn void _Exit(int ret) {
  __syscall1_noret(ret, SYS_EXIT_GROUP);
}

_Noreturn void abort() {
  _Exit(1);
}

_Noreturn void __stack_chk_fail(void);
_Noreturn void __stack_chk_fail(void) {
  // TODO: Output fail message.
  _Exit(42);
}

#define ABS(T, name) \
  T name(T n) { \
    if (n < 0) { \
      return -n; \
    } \
    return n; \
  }

ABS(int, abs)
ABS(long, labs)
ABS(long long, llabs)
ABS(intmax_t, imaxabs)

#define DIV(T, I, name) \
  T name(I x, I y) { \
    T ret; \
    ret.quot = x / y; \
    ret.rem = x % y; \
    return ret; \
  }

DIV(div_t, int, div)
DIV(ldiv_t, long, ldiv)
DIV(lldiv_t, long long, lldiv)
DIV(imaxdiv_t, intmax_t, imaxdiv)

static void swap(void *a, void *b, size_t size) {
  if (a == b) {
    return;
  }
  unsigned char buf[size];
  memcpy(buf, a, size);
  memcpy(a, b, size);
  memcpy(b, buf, size);
}

void qsort(void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *)) {
  unsigned char *arr = (unsigned char *)ptr;

  if (count <= 1) {
    return;
  }

  // Find partition value. We just use the last element.
  // TODO: Pick a better element.
  unsigned char *pivot = arr + ((count - 1) * size);
  // Partition by the value.
  unsigned char *right_side = arr;

  for (unsigned char *i = arr; i < pivot; i += size) {
    if (comp(i, pivot) < 0) {
      // This item goes on the left side of the partition.
      swap(i, right_side, size);
      right_side += size;
    }
  }

  // Insert the pivot to the left of the right side.
  swap(right_side, pivot, size);
  right_side += size;

  // Recurse on both halves.
  const size_t left_count = (right_side - arr) / size - 1;
  qsort(arr, left_count, size, comp);
  qsort(right_side, count - left_count - 1, size, comp);
}

void *bsearch(
    const void *key,
    const void *ptr,
    const size_t count,
    const size_t size,
    int (*comp)(const void *, const void *)) {
  const unsigned char *arr = (const void *)ptr;
  size_t min = 0;
  size_t max = count;

  while (min < max) {
    const size_t mid = (max - min) / 2 + min;
    const unsigned char *mid_ptr = arr + (mid * size);
    const int c = comp(mid_ptr, key);
    if (c < 0) {
      min = mid + 1;
    } else if (c > 0) {
      max = mid;
    } else {
      return (void *)mid_ptr;
    }
  }
  return NULL;
}

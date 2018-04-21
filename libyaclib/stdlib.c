#include "stdlib.h"

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

_Noreturn void __abort() {
  _Exit(1);
}

_Noreturn void abort() {
  __abort();
}

_Noreturn void __stack_chk_fail() {
  // TODO: Output fail message.
  _Exit(42);
}

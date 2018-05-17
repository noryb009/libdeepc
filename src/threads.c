#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <threads.h>

#include "syscall.h"
#include "threads_internal.h"

// TODO: Figure out how much space we need for thread local storage.
// For now, we allocate a static number of bytes.
#define TLS_SPACE 8*64

typedef void (*thrd_bootstrap_fn)(void);
_Noreturn void __thrd_bootstrap_asm(void);

// This represents the stack of the new thread when it starts.
typedef struct {
  // bootstrap_fn MUST be first.
  thrd_bootstrap_fn bootstrap_fn;
  thrd_start_t func;
  void *arg;
  thrd_t thr;
} thrd_bootstrap_info;

const size_t stack_size = 8 * 1024 * 1024; // 8MB stack.

static void free_thrd_t(thrd_t thr, bool free_stack) {
  if (free_stack) {
    free(thr->stack_base);
  }
  free(((unsigned char *)thr) - TLS_SPACE);
}

static thrd_t create_thrd_t(void) {
  unsigned char *thr_mem = malloc((TLS_SPACE + sizeof(struct __thrd_t_object)));
  if (!thr_mem) {
    return NULL;
  }
  thrd_t thr = (thrd_t)(thr_mem + TLS_SPACE);
  thr->self = thr;
  __spinlock_init(&thr->spin);
  thr->refs = 2; // Parent and child has a reference to the thread.
  return thr;
}

static _Noreturn void thrd_exit_with_thr(int ret, thrd_t thr) {
  __run_tss_destructors();

  __spinlock_lock(&thr->spin);
  if (thr->refs == 1) {
    // TODO: Free stack.
    free_thrd_t(thr, false);
  } else {
    assert(thr->refs == 2);
    thr->result = ret;
    thr->refs--;
    __spinlock_unlock(&thr->spin);
  }

  __syscall1_noret(0, SYS_EXIT);
}

static void setup_tls(thrd_t thr) {
  // Set up the FS register to point to the thread.
  if ((int64_t)__syscall2(ARCH_SET_FS, (uint64_t)thr, SYS_ARCH_PRCTL) < 0) {
    // Something is very wrong, exit.
    abort();
  }

  // Set up the thread's tss data structures.
  // TODO: When we do dynamic linking, make these regular initializations.
  __tss_data = NULL;
  __tss_data_capacity = 0;
}

_Noreturn void __thrd_bootstrap_c(thrd_bootstrap_info *info);
_Noreturn void __thrd_bootstrap_c(thrd_bootstrap_info *info) {
  // This was called from the thread bootstrapping assembly.

  setup_tls(info->thr);

  const int ret = info->func(info->arg);
  thrd_exit_with_thr(ret, info->thr);
}

int main(int argc, char **argv);
_Noreturn void __yac_main(int argc, char **argv) {
  thrd_t thr = create_thrd_t();
  if (!thr) {
    abort();
  }
  thr->stack_base = NULL;

  // TODO: This disallows using stack protectors.
  setup_tls(thr);

  const int result = main(argc, argv);
  free_thrd_t(thr, false);
  exit(result);
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
  // This function is based on the clone function, the same way it is used in:
  // https://nullprogram.com/blog/2015/05/15/

  *thr = create_thrd_t();
  if (*thr == NULL) {
    return thrd_nomem;
  }

  (*thr)->stack_base = malloc(stack_size);
  if ((*thr)->stack_base == NULL) {
    free_thrd_t(*thr, false);
    return thrd_nomem;
  }
  unsigned char *stack_top = (*thr)->stack_base + stack_size;
  // Push the info structure.
  stack_top -= sizeof(thrd_bootstrap_info);
  thrd_bootstrap_info *info = (thrd_bootstrap_info *)stack_top;

  // We need to make sure bootstrap_fn is the first thing in the structure.
  assert(info == (void *)&info->bootstrap_fn);

  // Add everything to the info structure.
  info->bootstrap_fn = __thrd_bootstrap_asm;
  info->func = func;
  info->arg = arg;
  info->thr = *thr;

  // TODO: CLONE_CHILD_CLEARTID?
  const uint64_t clone_flags =
    CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_PARENT | CLONE_THREAD | CLONE_PARENT_SETTID;

  const int64_t id = (int64_t)__syscall3(clone_flags, (uint64_t)stack_top, (uint64_t)(&(*thr)->id), SYS_CLONE);
  if (id < 0) {
    free_thrd_t(*thr, true);
    return thrd_error;
  }
  return thrd_success;
}

int thrd_equal(thrd_t lhs, thrd_t rhs) {
  return lhs == rhs;
}

void thrd_yield(void) {
  __syscall0(SYS_SCHED_YIELD);
}

_Noreturn void thrd_exit(int res) {
  thrd_exit_with_thr(res, thrd_current());
}

int thrd_detach(thrd_t thr) {
  __spinlock_lock(&thr->spin);
  thr->refs--;
  if (thr->refs == 0) {
    free_thrd_t(thr, true);
  } else {
    assert(thr->refs == 1);
    __spinlock_unlock(&thr->spin);
  }

  return thrd_success;
}

int thrd_join(thrd_t thr, int *res) {
  __spinlock_lock(&thr->spin);
  // If the thread has exited, it would have decreased thr->refs to 1.
  while (thr->refs == 2) {
    // Wait for exit.
    // TODO: Don't busy wait.
    __spinlock_unlock(&thr->spin);
    thrd_yield();
    __spinlock_lock(&thr->spin);
  }
  assert(thr->refs == 1);
  if (res != NULL) {
    *res = thr->result;
  }

  free_thrd_t(thr, true);
  return thrd_success;
}

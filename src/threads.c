#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <threads.h>

#include "spinlock.h"
#include "syscall.h"

typedef void (*thrd_bootstrap_fn)(void);
_Noreturn void __thrd_bootstrap_asm(void);

// This represents the stack of the new thread when it starts.
struct __thrd_t_object {
  __spinlock spin;
  unsigned char *stack_base;
  int id; // TODO: pid_t.
  int result;
  // TODO: Is `id` needed?
  signed char refs;
};

typedef struct {
  // bootstrap_fn MUST be first.
  thrd_bootstrap_fn bootstrap_fn;
  thrd_start_t func;
  void *arg;
  thrd_t thr;
} thrd_bootstrap_info;

const size_t stack_size = 8 * 1024 * 1024; // 8MB stack.

static _Noreturn void thrd_exit_with_thr(int ret, thrd_t thr) {
  __spinlock_lock(&thr->spin);
  // TODO: Clean up thr if last reference to it.
  if (thr->refs == 1) {
    // TODO: Free stack.
    free(thr);
  } else {
    thr->result = ret;
    thr->refs--;
    // TODO: Make sure no races here.
    __spinlock_unlock(&thr->spin);
  }

  __syscall1_noret(0, SYS_EXIT);
}

_Noreturn void __thrd_bootstrap_c(thrd_bootstrap_info *info);
_Noreturn void __thrd_bootstrap_c(thrd_bootstrap_info *info) {
  // This was called from the thread bootstrapping assembly.
  // TODO: Store thr in tls.
  const int ret = info->func(info->arg);
  thrd_exit_with_thr(ret, info->thr);
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
  // This function is based on the clone function, the same way it is used in:
  // https://nullprogram.com/blog/2015/05/15/
  *thr = malloc(sizeof(struct __thrd_t_object));
  if (*thr == NULL) {
    return thrd_nomem;
  }

  __spinlock_init(&(*thr)->spin);
  (*thr)->refs = 2; // Parent and child has a reference to the thread.

  (*thr)->stack_base = malloc(stack_size);
  if ((*thr)->stack_base == NULL) {
    free(*thr);
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
  // TODO: TLS stuff?

  const int64_t id = (int64_t)__syscall3(clone_flags, (uint64_t)stack_top, (uint64_t)(&(*thr)->id), SYS_CLONE);
  if (id < 0) {
    free((*thr)->stack_base);
    free(*thr);
    return thrd_error;
  }
  return thrd_success;
}

int thrd_equal(thrd_t lhs, thrd_t rhs) {
  return lhs == rhs;
}

thrd_t thrd_current(void) {
  // TODO: Use tls.
  return NULL;
}

void thrd_yield(void) {
  // TODO: sleep.
}

_Noreturn void thrd_exit(int res) {
  // TODO: Get thr.
  //thrd_exit_with_thr(result, thr);
  (void)res;
  abort();
}

int thrd_detach(thrd_t thr) {
  __spinlock_lock(&thr->spin);
  thr->refs--;
  if (thr->refs == 0) {
    free(thr->stack_base);
    free(thr);
  } else {
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
  if (*res != NULL) {
    *res = thr->result;
  }

  free(thr->stack_base);
  free(thr);
  return thrd_success;
}

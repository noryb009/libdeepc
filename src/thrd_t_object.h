#pragma once

#include <threads.h>

#include "spinlock.h"

struct __thrd_t_object {
  // As required, the first object in the thread object needs to be a pointer
  // to itself.
  struct __thrd_t_object *self;
  __spinlock spin;
  unsigned char *stack_base;
  int id; // TODO: pid_t.
  volatile int result;
  // TODO: Is `id` needed?
  volatile signed char refs;
};

extern thread_local void **__tss_data;
extern thread_local tss_t __tss_data_capacity;
void __run_tss_destructors(void);

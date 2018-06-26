#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <threads.h>

#include "spinlock.h"

struct __thrd_t_object {
  // As required, the first object in the thread object needs to be a pointer
  // to itself.
  struct __thrd_t_object *self; // 0x00
  unsigned char *stack_base;    // 0x08
  void *space_1;                // 0x10
  void *space_2;                // 0x18
  void *space_3;                // 0x20
  uintptr_t stack_protector;    // 0x28
  __spinlock spin;
  int id; // TODO: pid_t.
  volatile int result;
  // TODO: Is `id` needed?
  volatile signed char refs;
};
static_assert(offsetof(struct __thrd_t_object, stack_protector) == 0x28,
              "Invalid stack protector offset.");

// The function run before main().
_Noreturn void __libc_start_main(int argc, char **argv);

extern thread_local void **__tss_data;
extern thread_local tss_t __tss_data_capacity;
void __run_tss_destructors(void);

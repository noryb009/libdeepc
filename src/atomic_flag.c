#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

_Bool __atomic_flag_set_relaxed(volatile atomic_flag *flag);
_Bool __atomic_flag_set_consume(volatile atomic_flag *flag);
_Bool __atomic_flag_set_acquire(volatile atomic_flag *flag);
_Bool __atomic_flag_set_release(volatile atomic_flag *flag);
_Bool __atomic_flag_set_acq_rel(volatile atomic_flag *flag);
_Bool atomic_flag_test_and_set(volatile atomic_flag *flag);

void __atomic_flag_clear_relaxed(volatile atomic_flag *flag);
void __atomic_flag_clear_consume(volatile atomic_flag *flag);
void __atomic_flag_clear_acquire(volatile atomic_flag *flag);
void __atomic_flag_clear_release(volatile atomic_flag *flag);
void __atomic_flag_clear_acq_rel(volatile atomic_flag *flag);
void atomic_flag_clear(volatile atomic_flag *flag);

_Bool atomic_flag_test_and_set_explicit(volatile atomic_flag *flag, memory_order order) {
  switch (order) {
    case memory_order_relaxed:
      return __atomic_flag_set_relaxed(flag);
    case memory_order_consume:
      return __atomic_flag_set_consume(flag);
    case memory_order_acquire:
      return __atomic_flag_set_acquire(flag);
    case memory_order_release:
      return __atomic_flag_set_release(flag);
    case memory_order_acq_rel:
      return __atomic_flag_set_acq_rel(flag);
    case memory_order_seq_cst:
      return atomic_flag_test_and_set(flag);
  }
  abort();
}

void atomic_flag_clear_explicit(volatile atomic_flag *flag, memory_order order) {
  switch (order) {
    case memory_order_relaxed:
      __atomic_flag_clear_relaxed(flag);
      break;
    case memory_order_consume:
      __atomic_flag_clear_consume(flag);
      break;
    case memory_order_acquire:
      __atomic_flag_clear_acquire(flag);
      break;
    case memory_order_release:
      __atomic_flag_clear_release(flag);
      break;
    case memory_order_acq_rel:
      __atomic_flag_clear_acq_rel(flag);
      break;
    case memory_order_seq_cst:
      atomic_flag_clear(flag);
      break;
  }
}

#include <stdatomic.h>
#include <stdbool.h>

_Bool atomic_flag_test_and_set(volatile atomic_flag* flag) {
  return atomic_flag_test_and_set_explicit(flag, memory_order_seq_cst);
}

void atomic_flag_clear(volatile atomic_flag* flag) {
  atomic_flag_clear_explicit(flag, memory_order_seq_cst);
}

void atomic_flag_clear_explicit(volatile atomic_flag* flag, memory_order order) {
  // TODO: Handle memory_order.
  (void)order;
  *flag = false;
}

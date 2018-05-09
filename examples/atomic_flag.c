#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>

// TODO: Test with multiple threads.

int main(void) {
  atomic_flag flag = ATOMIC_FLAG_INIT;
  assert(atomic_flag_test_and_set(&flag) == false);
  assert(atomic_flag_test_and_set(&flag) == true);
  assert(atomic_flag_test_and_set(&flag) == true);
  atomic_flag_clear(&flag);
  assert(atomic_flag_test_and_set(&flag) == false);
}

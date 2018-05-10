#include <stdbool.h>

#include "spinlock.h"

void __spinlock_init(volatile __spinlock *spinlock) {
  // TODO: Use atomic init function (when it is written).
  *spinlock = ATOMIC_FLAG_INIT;
}

void __spinlock_lock(volatile __spinlock *spinlock) {
  atomic_flag_clear(spinlock);
  while (atomic_flag_test_and_set(spinlock) == true) {
    // Busy wait.
  }
}

void __spinlock_unlock(volatile __spinlock *spinlock) {
  atomic_flag_clear(spinlock);
}

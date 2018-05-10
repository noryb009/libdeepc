#include <threads.h>

#include "spinlock.h"

/* We currently implement mutexes as spinlocks, since they are easy.
 * When we switch to a better implementation, we will still need spinlocks.
 */

struct mtx_t {
  volatile __spinlock spin;
};

int mtx_init(mtx_t *mutex, int type) {
  // TODO: Handle type.
  __spinlock_init(mutex->spin);
  return thrd_success;
}

int mtx_lock(mtx_t *mutex) {
  __spinlock_lock(mutex->spin);
  return thrd_success;
}

int mtx_trylock(mtx_t *mutex) {
  // TODO: Wait.
  __spinlock_lock(mutex->spin);
  return thrd_success;
}

int mtx_unlock(mtx_t *mutex) {
  __spinlock_unlock(mutex->spin);
  return thrd_success;
}

void mtx_destroy(mtx_t *mutex) {
  // Nothing. Hopefully nobody is waiting.
}

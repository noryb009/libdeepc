#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <threads.h>

#include "thrd_t_object.h"

// From "ELF Handling For Thread-Local Storage", by Ulrich Drepper.
typedef struct {
  unsigned long int ti_module;
  unsigned long int ti_offset;
} tls_index;

void *__tls_get_addr(tls_index *ti);
void *__tls_get_addr(tls_index *ti) {
  (void)ti;
  // TODO: Implement this when linking dynamic libraries are supported.
  return NULL;
}

// We need a mutex.
static volatile atomic_flag tss_mtx_flag = ATOMIC_FLAG_INIT;
static volatile bool tss_mtx_init = false;
static mtx_t tss_mtx;

// tss_next is a ticket counter.
// TODO: Implement a free list in the dtor array. If we do this, tss_t should
// probably be a uintptr_t.
static tss_t tss_next = 0;

// TODO: There's a similar function in malloc.c, this should be generic-ized.
static void init_tss_mtx_flag(void) {
  if (tss_mtx_init) {
    return;
  }

  while (atomic_flag_test_and_set(&tss_mtx_flag) == true) {
    // Busy wait.
  }

  mtx_init(&tss_mtx, mtx_plain);
  // TODO: Do we need a barrier here?
  tss_mtx_init = true;

  atomic_flag_clear(&tss_mtx_flag);
}

int tss_create(tss_t *tss_key, tss_dtor_t dtor) {
  // TODO: Handle dtor.
  (void)dtor;

  init_tss_mtx_flag();

  mtx_lock(&tss_mtx);
  *tss_key = tss_next;
  tss_next++;
  mtx_unlock(&tss_mtx);

  return thrd_success;
}

void *tss_get(tss_t tss_key) {
  // TODO.
}

int tss_set(tss_t tss_id, void *val) {
  // TODO.
}

void tss_delete(tss_t tss_id) {
  // TODO.
}

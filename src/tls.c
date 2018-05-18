#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#include "threads-internal.h"

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

typedef struct {
  tss_dtor_t dtor;
} tss_meta_t;

// We need a mutex for the global variables.
// TODO: Make this a reader-writer lock.
static volatile atomic_flag tss_mtx_flag = ATOMIC_FLAG_INIT;
static volatile bool tss_mtx_init = false;
static mtx_t tss_mtx;

thread_local void **__tss_data;
thread_local tss_t __tss_data_capacity;

// `tss_meta` is a dynamic array holding global tss meta information, tracking
// what IDs correspond to what dtors.
// `tss_next` is a ticket counter, counting what tss ID should be given out
// next.
static tss_meta_t *tss_meta = NULL;
static size_t tss_meta_capacity = 0;
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
  init_tss_mtx_flag();

  mtx_lock(&tss_mtx);

  // Make the dynamic array larger, if needed.
  if (tss_meta_capacity <= tss_next) {
    const size_t new_capacity = (tss_meta_capacity == 0) ? 8 : (tss_meta_capacity * 2);
    tss_meta_t * const new_tss_meta = malloc(new_capacity * sizeof(tss_meta_t));
    if (!new_tss_meta) {
      mtx_unlock(&tss_mtx);
      return thrd_error;
    }

    if (tss_meta) {
      memcpy(new_tss_meta, tss_meta, tss_meta_capacity * sizeof(tss_meta_t));
      free(tss_meta);
    }

    tss_meta = new_tss_meta;
    tss_meta_capacity = new_capacity;
  }

  *tss_key = tss_next;
  tss_next++;

  tss_meta[*tss_key].dtor = dtor;
  mtx_unlock(&tss_mtx);

  return thrd_success;
}

void *tss_get(tss_t tss_key) {
  if (__tss_data_capacity <= tss_key) {
    return NULL;
  }
  return __tss_data[tss_key];
}

int tss_set(tss_t tss_key, void *val) {
  if (__tss_data_capacity <= tss_key) {
    // Allocate up to `tss_meta_capacity`, which is the next power of 2 above
    // `tss_next`.
    mtx_lock(&tss_mtx);
    const size_t new_capacity = tss_meta_capacity;
    mtx_unlock(&tss_mtx);
    assert(new_capacity > tss_key);

    // Resize the array.
    static_assert(NULL == 0, "NULL is not 0");
    void ** const new_tss_data = calloc(new_capacity, sizeof(void *));
    if (new_tss_data == NULL) {
      return thrd_error;
    }
    if (__tss_data != NULL) {
      memcpy(new_tss_data, __tss_data, __tss_data_capacity * sizeof(void *));
      free(__tss_data);
    }
    __tss_data = new_tss_data;
    __tss_data_capacity = new_capacity;
  }

  __tss_data[tss_key] = val;
  return thrd_success;
}

void tss_delete(tss_t tss_key) {
  // Note: tss_key came from tss_create, so the mutex is initialized.
  mtx_lock(&tss_mtx);
  tss_meta[tss_key].dtor = NULL;
  mtx_unlock(&tss_mtx);
}

void __run_tss_destructors(void) {
  init_tss_mtx_flag();

  // This function is a bit messy because dtors can call tss_delete.

  mtx_lock(&tss_mtx);
  // Look for a dtor we can run.
  for (tss_t i = 0; i < __tss_data_capacity; ++i) {
    if (tss_meta[i].dtor != NULL && __tss_data[i] != NULL) {
      // Copy the dtor to make sure it isn't deleted after releasing the lock.
      const tss_dtor_t dtor = tss_meta[i].dtor;
      // We need to release the lock so we can recurse on tss_delete.
      // TODO: Can we use a multi-acquire mutex?
      mtx_unlock(&tss_mtx);
      dtor(__tss_data[i]);
      mtx_lock(&tss_mtx);
    }
  }
  mtx_unlock(&tss_mtx);
}

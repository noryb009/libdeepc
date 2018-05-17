#pragma once

#include <stdinternaltypes.h>

typedef struct __thrd_t_object *thrd_t;

typedef int (*thrd_start_t)(void *);

enum {
  thrd_success = 0,
  thrd_nomem = 1,
  thrd_timedout = 2,
  thrd_busy = 3,
  thrd_error = 4,
};

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
int thrd_equal(thrd_t lhs, thrd_t rhs);
thrd_t thrd_current(void);
// TODO: thrd_sleep
void thrd_yield(void);
_Noreturn void thrd_exit(int res);
int thrd_detach(thrd_t thr);
int thrd_join(thrd_t thr, int *res);

// Mutexes.
typedef struct {
  __atomic_flag spin;
} mtx_t;

enum {
  mtx_plain = 1 << 0,
  mtx_recursive = 1 << 1,
  mtx_timed = 1 << 2,
};

int mtx_init(mtx_t *mutex, int type);
int mtx_lock(mtx_t *mutex);
// TODO: mtx_timedlock.
int mtx_trylock(mtx_t *mutex);
int mtx_unlock(mtx_t *mutex);
void mtx_destroy(mtx_t *mutex);

// TODO: Call once.

// Thread local storage.

#define thread_local _Thread_local

// Thread specific storage.

// TODO Make larger.
#define TSS_DTOR_ITERATIONS 1
typedef void (*tss_dtor_t)(void *);

typedef __uintptr_t tss_t;

int tss_create(tss_t *tss_key, tss_dtor_t dtor);
void *tss_get(tss_t tss_key);
int tss_set(tss_t tss_key, void *val);
void tss_delete(tss_t tss_key);

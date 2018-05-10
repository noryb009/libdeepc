#pragma once

#include <stdatomic.h>

typedef atomic_flag __spinlock;

void __spinlock_init(volatile __spinlock *spinlock);
void __spinlock_lock(volatile __spinlock *spinlock);
void __spinlock_unlock(volatile __spinlock *spinlock);

/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2015-2020 Micron Technology, Inc.  All rights reserved.
 */

#ifndef HSE_PLATFORM_SPINLOCK_H
#define HSE_PLATFORM_SPINLOCK_H

#include <pthread.h>
#include <assert.h>

typedef struct {
    pthread_spinlock_t lock;
} spinlock_t;

/*
 * NOTE: pthreads does not provide a static initializer for
 * pthread_spinlock_t, but the GNU C Library implemenation of pthreads
 * uses a value of 1 for the unlocked state.
 */
#define DEFINE_SPINLOCK(xx) spinlock_t xx = {.lock = 1 }

static inline void
spin_lock_init(spinlock_t *lock)
{
    int rc __maybe_unused;

    rc = pthread_spin_init(&lock->lock, PTHREAD_PROCESS_PRIVATE);
    assert(rc == 0);
}

static inline void
spin_lock(spinlock_t *lock)
{
    int rc __maybe_unused;

    rc = pthread_spin_lock(&lock->lock);
    assert(rc == 0);
}

static inline int
spin_trylock(spinlock_t *lock)
{
    int rc;

    rc = pthread_spin_trylock(&lock->lock);

    return rc ? 0 : 1;
}

static inline void
spin_unlock(spinlock_t *lock)
{
    int rc __maybe_unused;

    rc = pthread_spin_unlock(&lock->lock);
    assert(rc == 0);
}

static inline void
spin_lock_irqsave(spinlock_t *lock, unsigned long flags __maybe_unused)
{
    spin_lock(lock);
}

static inline void
spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags __maybe_unused)
{
    spin_unlock(lock);
}

#endif

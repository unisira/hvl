#ifndef HVL_MUTEX_H
#define HVL_MUTEX_H

#include <Wdm.h>

typedef struct
{
    FAST_MUTEX _inner_mutex;
} hvl_mutex_t;

static inline void hvl_initialise_mutex(hvl_mutex_t* mutex)
{
    ExInitializeFastMutex(mutex->_inner_mutex);
}

static inline void hvl_acquire_mutex(hvl_mutex_t* mutex)
{
    ExAcquireFastMutex(&mutex->_inner_mutex);
}

static inline void hvl_release_mutex(hvl_mutex_t* mutex)
{
    ExReleaseFastMutex(&mutex->_inner_mutex);
}

#endif

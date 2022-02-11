#ifndef BASE_MUTEXLOCK_H
#define	BASE_MUTEXLOCK_H

#include "noncopyable.h"
#include <pthread.h>
#include <assert.h>

class MutexLock : public noncopyable
{
public:
	MutexLock()
	{
		assert(pthread_mutex_init(&mutex_, NULL) == 0);
	}

	pthread_mutex_t *getPthreadMutex()
	{
		return &mutex_;
	}
	
	void lock(void)
	{
		assert(pthread_mutex_lock(&mutex_) == 0);
	}

	void unlock(void)	
	{
		assert(pthread_mutex_unlock(&mutex_) == 0);
	}

	~MutexLock()
	{
		assert(pthread_mutex_destroy(&mutex_) == 0);
	}

private:
	pthread_mutex_t mutex_;
};

#endif
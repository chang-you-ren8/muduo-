#ifndef BASE_CONDITION_H
#define BASE_CONDITION_H

#include "MutexLock.h"
#include "noncopyable.h"

#include <pthread.h>
#include <assert.h>

class Condition : public noncopyable
{
public:
	Condition(MutexLock &mutex)
		:mutex_(mutex)
	{
		assert(pthread_cond_init(&cond_, NULL) == 0);
	}

	void wait(void)
	{
		assert(pthread_cond_wait(&cond_, mutex_.getPthreadMutex()) == 0);
	}
	
	void notify(void)
	{
		assert(pthread_cond_signal(&cond_) == 0);		
	}

	void notifyAll(void)
	{
		assert(pthread_cond_broadcast(&cond_) == 0);
	}

	~Condition()
	{
		assert(pthread_cond_destroy(&cond_) == 0);
	}

private:
	MutexLock &mutex_;
	pthread_cond_t cond_;
};

#endif
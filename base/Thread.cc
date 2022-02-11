#include "Thread.h"
#include <pthread.h>
#include <assert.h>

Thread::Thread(ThreadFunc run)
	:run_(std::move(run)), tid_(0), started_(false), joined_(false)
{
	
}

void Thread::start()
{
	assert(!started_);
	started_ = true;
	assert(pthread_create(&tid_, NULL, startThread, this) == 0);
}

void Thread::join()
{
	assert(started_);
	assert(!joined_);
	joined_ = true;
	assert(pthread_join(tid_, NULL) == 0);
}


void *Thread::startThread(void *arg)
{
	Thread *thread = static_cast<Thread *>(arg);
	thread->runInThread();
	return NULL;
}

void Thread::runInThread()
{
	run_();
}

Thread::~Thread()
{
	if(started_ && !joined_)	
		assert(pthread_detach(tid_) == 0);
}


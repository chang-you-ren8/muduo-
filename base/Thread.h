#ifndef _THREAD_H
#define _THREAD_H

#include<functional>
#include "noncopyable.h"

class Thread : public noncopyable
{
public:
	using ThreadFunc = std::function<void(void)>;

	explicit Thread(ThreadFunc run);
	
	void start();
	void join();

	~Thread();	
private:
	ThreadFunc run_;
	pthread_t tid_;
	bool started_;
	bool joined_;

	static void *startThread(void *arg);
	void runInThread();
};

#endif

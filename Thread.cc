#include "Thread.h"
#include "CurrentThread.h"

#include <assert.h>

Thread::Thread(ThreadFunc func)
    :   func_(std::move(func)),
        started_(false),
        joined_(false),
        tid_(0),
        latch_(1)
{

}


Thread::~Thread()
{
    if(started_ && !joined_)
    {
        int n = pthread_detach(pthreadId_);
        assert(n == 0);
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;

    detail::ThreadData *data = new detail::ThreadData(func_, &tid_, &latch_);
    int ret = pthread_create(&pthreadId_, NULL, startInThread, data);
    assert(ret == 0);

    //等待tid_初始化完成
    latch_.wait();
    assert(tid_ > 0);
}

void Thread::join()
{
    if(started_ && !joined_)
    {
        joined_ = true;
        int ret = pthread_join(pthreadId_, NULL);
        assert(ret == 0);
    }
}

void *Thread::startInThread(void *arg)
{
    detail::ThreadData *threadData = static_cast<detail::ThreadData *>(arg);
    *(threadData->tid_) = CurrentThread::tid();  
    threadData->latch_->countDown();

    ThreadFunc func = threadData->func_;
    //数据要在这里释放bug...
    delete threadData;
    func();


    return NULL;
}

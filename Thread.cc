#include "Thread.h"
#include "CurrentThread.h"


//ThreadData
//为什么使用ThreadData呢？主要因为pthread_create不能传递类的非静态成员函数
//当然你可以使用静态成员函数
class ThreadData : noncopyable
{
public:
    using ThreadFunc = Thread::ThreadFunc;

    ThreadData(const ThreadFunc &cb, pid_t *tid, CountDownLatch *latch)
        :   func_(cb),
            tid_(tid),
            latch_(latch)
    {

    }

    ThreadFunc func_;
    pid_t *tid_;
    CountDownLatch *latch_;
};

void *runInThread(void *arg)
{
    ThreadData *threadData = static_cast<ThreadData *>(arg);

    *threadData->tid_ =  CurrentThread::tid();
    threadData->latch_->countDown();            //这个时候其他线程才能访问Thread的tid
    ThreadData::ThreadFunc cb = threadData->func_;

    delete threadData;

    cb();
}

Thread::Thread(const ThreadFunc &cb)
    :   func_(cb),
        tid_(0),
        latch_(1),
        running_(false),
        joined_(false)
{

}

Thread::~Thread()
{
    //为什么不用pthread_join呢？
    //因为pthread_join会等待线程终止
    if(running_ && !joined_)
        assert(pthread_detach(pthreadId_) == 0);
}

void Thread::start()
{
    ThreadData *threadData = new ThreadData(func_, &tid_, &latch_);
    if(pthread_create(&pthreadId_, NULL, runInThread, threadData))
    {
        delete threadData;
        fprintf(stderr, "%s\n", "Thread::start pthread_create failed");
    }
    else
    {
        running_ = true;
        latch_.wait();      //等待tid_初始化完成
    }
}

bool Thread::started()
{
    return running_;
}

void Thread::join()
{
    if(running_ && !joined_)
    {
        joined_ = true;
        assert(pthread_join(pthreadId_, NULL) == 0);
    }
}

pid_t Thread::tid() const 
{
    return tid_;
}
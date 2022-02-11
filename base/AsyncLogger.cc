#include "AsyncLogger.h"
#include "LogFile.h"

#include <assert.h>

#include <functional>

AsyncLogger::AsyncLogger(const std::string &basename,
                        off_t rollSize,
                        time_t flushInterval)
    : basename_(basename),
      rollSize_(rollSize),
      flushInterval_(flushInterval),
      running_(false),
      mutex_(),
      cond_(mutex_),
      thread_(std::bind(&AsyncLogger::threadFunc, this)), //为什么会出现invalid use non-static member function
      curBuffer_(new Buffer),
      nxtBuffer_(new Buffer),
      buffers_()
{
    buffers_.reserve(16);
}

AsyncLogger::~AsyncLogger()
{
    if(running_)
        stop();
}

//这个必须加锁，因为可能存在多个生产者
void AsyncLogger::append(const char *msg, int len)
{
    MutexLockGuard guard(mutex_);
    assert(curBuffer_);
    if(curBuffer_->avail() >= len)
    {
        curBuffer_->append(msg, len);
    }
    else 
    {
        buffers_.push_back(std::move(curBuffer_));
        if(nxtBuffer_)
            curBuffer_ = std::move(nxtBuffer_);
        else 
            curBuffer_.reset(new Buffer);

        curBuffer_->append(msg, len);
        cond_.notify(); //buffers_非空
    }
}

void AsyncLogger::start()
{
    if(!running_)
    {
        running_ = true;
        thread_.start();
    }
}

void AsyncLogger::stop()
{
    if(running_)
    {
        running_ = false;
        cond_.notify();
        thread_.join();
    }
}

void AsyncLogger::threadFunc()
{
    assert(running_);

    //日志线程（消费者）使用的缓冲区
    //如果curBuffer_为空，那么将buffer1移动给curBuffer_
    //如果nxtBuffer_为空，那么将buffer2移动给nxtBuffer_
    BufferPtr buffer1(new Buffer);
    BufferPtr buffer2(new Buffer);
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);

    //妈的第一下LogFile接口没有设计好，炸了。。。。
    //话说这个默认参数太多也不好啊？？？？？(能不能指定参数呢？)
    //设计多个构造函数？？？
    LogFile outputFile(basename_, rollSize_, flushInterval_, false);

    while(running_)
    {
       assert(buffer1 && buffer1->size() == 0);
       assert(buffer2 && buffer2->size() == 0);
       assert(buffersToWrite.empty());

        {
            MutexLockGuard guard(mutex_);
            if(buffers_.empty())
                cond_.wait();

            buffers_.push_back(std::move(curBuffer_));
            curBuffer_ = std::move(buffer1);
            if(!nxtBuffer_)
                nxtBuffer_ = std::move(buffer2);
            buffersToWrite.swap(buffers_);
        }

        assert(!buffersToWrite.empty());

        //日志太多如何处理，这里只是为了学习muduo的设计，所以暂时不考虑这个问题
        if(buffersToWrite.size() >= 25)
        {
            fprintf(stderr, "%s\n", "to much log message");
            buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
            buffersToWrite.resize(2);
        }

        for(const auto &bptr : buffersToWrite)
            outputFile.append(bptr->data(), bptr->size());
        
        if(!buffer1)
        {
            buffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            buffer1->reset();
        }
        if(!buffer2)
        {
            assert(!buffersToWrite.empty());
            buffer2 = std::move(buffersToWrite.back());
            buffer2->reset();
        }

        buffersToWrite.clear();
        outputFile.flush();
    }
    outputFile.flush();
}
#ifndef BASE_ATOMICINTEGER_H
#define BASE_ATOMICINTEGER_H

#include "noncopyable.h"

#include <stdint.h>

//为什么不可以拷贝？
//拷贝会破坏原子性的含义？
//多线程计数
template<typename T>
class AtomicInteger : noncopyable 
{
public:
    AtomicInteger()
        :   val_(0)
    { }

    T get()
    {
        return __sync_val_compare_and_swap(&val_, 0, 0);
    }

    T getAndAdd(T x)
    {
        return __sync_fetch_and_add(&val_, x);
    }

    T addAndGet(T x)
    {
        return x + getAndAdd(x);
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T getAndIncrement()
    {
        return getAndAdd(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    T getAndDecrement()
    {
        return getAndAdd(-1);
    }

    void add(T x)
    {
        getAndAdd(x);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        decrementAndGet();
    }

    T getAndSet(T newVal)
    {
        return __sync_lock_test_and_test(&val_, newVal);
    }

private:
    volatile T val_;        //特种内存定义：：：：：：：
};

using AtomicInt64_t = AtomicInteger<int64_t>;
using AtomicInt32_t = AtomicInteger<int32_t>;

#endif 

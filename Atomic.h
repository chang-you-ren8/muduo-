#ifndef BASE_ATOMIC_H
#define BASE_ATOMIC_H

//这个原子操作是利用gcc提供的内置函数来实现的。
//编译时要加上-march=native
//native表示自动判断机器类型

//原子增加
//type __sync_fetch_and_add(type *ptr, type value)
//原子比较和交换
//type __sync_val_compare_and_swap(type *ptr, type oldval, type newval)
//原子赋值
//type __sync_lock_test_and_set(type *ptr, type value)
#include "noncopyable.h"

#include <stdint>

template<typename T>
class AtomicInteger : noncopyable
{
public:
    AtomicInteger()
        :   value_(0)
    {

    }

    T get()
    {
        //先返回value_的当前值
        //然后如果value_为0，那么将value_设置为0
        //原子比较和交换
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T getAndAdd(T x)
    {
        //原子增加
        return __sync_fetch_and_add(&value, x);
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        decrementAndGet();
    }

    T getAndSet(T newValue)
    {
        //原子赋值操作
        return __sync_lock_test_and_set(&value_, newValue);
    }

private:
    volatile T value_;
};

using AtomicInt32 = AtomicInteger<int32_t>;
using AtommicInt64 = AtomicInteger<int64_t>;

#endif 
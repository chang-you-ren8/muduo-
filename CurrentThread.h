#ifndef BASE_CURRENTTHREAD_H
#define BASE_CURRENTTHREAD_H

//t_tid用作线程对应的pid_t缓存
extern __thread pid_t t_tid; //__thread每个线程独有的

namespace CurrentThread
{
    pid_t tid();
};

#endif
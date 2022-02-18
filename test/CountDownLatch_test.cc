#include "CountDownLatch.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <vector>
using std::vector;

const int N = 5;
//等待N个子线程输出完之后主线程再执行
CountDownLatch g_latch(N);

void *run(void *arg)
{
    sleep(5);

    int *v = (int *)arg;
    printf("run %d\n", *v);
    delete v;

    g_latch.countDown();

    return NULL;
}

int main()
{
    vector<pthread_t> pv(N);
    for(int i = 0; i < N; ++i)
    {
        int *v = new int(i);
        pthread_create(&pv[i], NULL, run, v);
    }

    g_latch.wait();
    //必定最后输出
    printf("in main()\n");

    return 0;
}
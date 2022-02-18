#include "Singleton.h"

#include <vector>

#include <pthread.h>
#include <stdio.h>

using namespace std;

class Test
{
public:
    Test()
    {
        //应该只输出一次
        printf("in Test::Test()\n");
    }
};

void* run(void *arg)
{
    for(int i = 0; i < 100; ++i)
    {
        Singleton<Test>::instance();
    }
    return NULL;
}

int main()
{
    vector<pthread_t> pv(5);
    for(auto &idx : pv)
        pthread_create(&idx, NULL, run, NULL);

    for(const auto &idx : pv)
        pthread_join(idx, NULL);

    return 0;
}
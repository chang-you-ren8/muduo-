#include "AsyncLogger.h"
#include "Logger.h"
#include "Thread.h"

#include <unistd.h>

#include <memory>
#include <vector>

using std::vector;
using std::unique_ptr;

unique_ptr<AsyncLogger> g_file;

void outputFunc(const char *logline, int len)
{
    g_file->append(logline, len);
}

void threadFunc()
{
	int tid = static_cast<int>(pthread_self());

    int N = 10000;
    for(int i = 0; i < N; ++i)
    {
        LOG_INFO << "this thread: " << tid;
        usleep(1);
    }
}

int main(int argc, char *argv[])
{
    Logger::setOutputFunc(outputFunc);
    Logger::setLogLevel(Logger::INFO);

    off_t rollSize = 10 * 1000;
    char name[128];
    strncpy(name, argv[0], sizeof(name));
    g_file.reset(new AsyncLogger(::basename(name), rollSize));
    g_file->start();

    int threadNum = 5;
    vector<unique_ptr<Thread>> threads;
    for(int i = 0; i < threadNum; ++i)
        threads.emplace_back(new Thread(threadFunc));
    for(auto &t :threads) 
        t->start();

    //我TM之前没加这句话一直segmentfault。
    //为什么呢？如果日志线程已经退出了那么日志线程中的数据已经无效了，所以日志线程一定最后才退出
    for(auto &t : threads)
       t->join();

    return 0;
}

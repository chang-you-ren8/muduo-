#include "LogFile.h"
#include "Logger.h"

#include <string.h>
#include <unistd.h>

#include <memory>

std::unique_ptr<LogFile> g_file;

void outputFunc(const char *logline, int len)
{
    g_file->append(logline, len);
}

void flushFunc()
{
    g_file->flush();
}

int main(int argc, const char *argv[])
{
    char name[256] = {0};
    strncpy(name, argv[0], sizeof(name) - 1);
    g_file.reset(new LogFile(::basename(name), 1000 * 1000));
    Logger::setOutputFunc(outputFunc);
    Logger::setFlushFunc(flushFunc);

    const char *str = "123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIGKLMNOPQRSTUVWXYZ";
    const int S = 1000 * 1000;

    for(int i = 0; i < S; ++i)
        LOG_INFO << str << i;

    return 0;
}
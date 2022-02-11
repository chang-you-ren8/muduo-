#ifndef BASE_PROCESSINFO_H
#define BASE_PROCESSINFO_H

#include <unistd.h>
#include <sys/types.h>

#include <string>

class ProcessInfo   
{
public:
    static int pid() { return static_cast<int>(::getpid()); }

    static std::string hostname()
    {
        char name[128];
        ::gethostname(name, sizeof(name));
        return std::string(name);
    }
};

#endif 
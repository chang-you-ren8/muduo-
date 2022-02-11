#ifndef BASE_APPENDFILE_H
#define BASE_APPENDFILE_H 

#include "noncopyable.h"

#include <unistd.h>

#include <stdio.h>

//not thread safe
//对外提供append 和 flush函数
class AppendFile : noncopyable
{
public:
    explicit AppendFile(const char *filename);
    ~AppendFile();

    void append(const char *msg, size_t len);
    void flush();

    off_t writtenBytes() const { return writtenBytes_; }
private:
    size_t write(const char *msg, size_t len);
    
    FILE *fp_;
    off_t writtenBytes_;
};
#endif 
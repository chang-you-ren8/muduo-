#ifndef BASE_LOGSTREAM_H
#define BASE_LOGSTREAM_H

#include "noncopyable.h"
#include "Timestamp.h"
#include "FixedBuffer.h"

#include <string>

class LogStream : noncopyable
{
public:
    using Buffer = SmallFixedBuffer;
    using self = LogStream;

    LogStream(Timestamp tim, const char *logLevelName, const char *basename, int line, int errNum);
    ~LogStream() = default;

    self &operator<<(char v);
    self &operator<<(short v);
    self &operator<<(unsigned short v);
    self &operator<<(int v);
    self &operator<<(unsigned int v);
    self &operator<<(long v);
    self &operator<<(unsigned long v);
    self &operator<<(long long v);
    self &operator<<(unsigned long long v);

    self &operator<<(float v);
    self &operator<<(double v);

    self &operator<<(const char *msg);
    self &operator<<(const std::string &msg);

    void finish();
    const Buffer &data() { return buffer_; }
private:
    Buffer buffer_;
    const char *basename_;
    const int line_;
    const int errNum_;
};

#endif 
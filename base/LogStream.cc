#include "LogStream.h"

#include <string.h>

#include <algorithm>

LogStream::LogStream(Timestamp tim, const char *logLevelName, const char *basename, int line, int errNum)
    : buffer_(),
      basename_(basename),
      line_(line),
      errNum_(errNum)
{
    *this << tim.toDate(true) << "--" << logLevelName << "--";
}

void LogStream::finish()
{
    *this << "--" << basename_ << ':' << line_;
    if(errNum_)
    {
        char buf[256] = {0};
        *this << "--" << strerror_r(errNum_, buf, sizeof(buf));
    }
    *this << '\n';
}

const char *D = "9876543210123456789";
const char *digits = D + 9;
const int kNumericMaxSize = 48;

template<typename T>
int convert(T v, char *buf)
{
    char *p = buf;
    T t = v;

    do
    {
        *p++ = digits[t % 10];
        t /= 10;
    }while(t != 0);

    if(v < 0)
        *p++ = '-';
    *p = '\0';

    std::reverse(buf, p);
    return static_cast<int>(p - buf);
}

LogStream &LogStream::operator<<(char v)
{
    const char *s = &v;
    *this << s;
    return *this;
}

LogStream &LogStream::operator<<(const char *s)
{
    const int len = static_cast<int>(strlen(s));
    if(buffer_.avail() >= len)
    {
        char *p = buffer_.current();
        strncpy(p, s, len);
        buffer_.add(len);
    }
    return *this;
}

LogStream &LogStream::operator<<(const std::string &msg)
{
    *this << msg.c_str();
    return *this;
}

LogStream &LogStream::operator<<(int v)
{
    if(buffer_.avail() >= kNumericMaxSize)
    {
        int len = convert(v, buffer_.current());
        buffer_.add(len);
    }
    return *this;
}

LogStream &LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream &LogStream::operator<<(unsigned int v)
{
    if(buffer_.avail() >= kNumericMaxSize)
    {
        int len = convert(v, buffer_.current());
        buffer_.add(len);
    }
    return *this;
}

LogStream &LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}
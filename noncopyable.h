#ifndef BASE_NONCOPYABEL_H
#define BASE_NONCOPYABEL_H

class noncopyable
{
public:
    noncopyable(const noncopyable &rhs) = delete;
    noncopyable &operator=(const noncopyable &rhs) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

#endif 
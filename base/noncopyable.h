#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

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

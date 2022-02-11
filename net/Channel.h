#ifndef _NET_CHANNEL_H_
#define _NET_CHANNEL_H_

#include "base/noncopyable.h"
#include "base/Timestamp.h"
#include "EventLoop.h"

#include <poll.h>

#include <functional>

class Channel: public noncopyable
{
public:
	using EventCallback = std::function<void(void)>;
	using ReadEventCallback = std::function<void(Timestamp)>;


	Channel(EventLoop *loop, int fd);

	~Channel() = default;

	
	EventLoop *ownerLoop()const
	{
		return loop_;
	}

	int fd()const
	{
		return fd_;
	}

	int index()const
	{
		return index_;
	}

	void setIndex(int index)
	{
		index_ = index;
	}

	int events()const
	{
		return events_;
	}

	void setRevents(int revents)
	{
		revents_ = revents;
	}

	
	void setReadCallback(ReadEventCallback cb)
	{
		readCallback_ = std::move(cb);
	}

	void setWriteCallback(EventCallback cb)
	{
		writeCallback_ = std::move(cb);
	}

	void setCloseCallback(EventCallback cb)
	{
		closeCallback_ = std::move(cb);
	}

	void setErrorCallback(EventCallback cb)
	{
		errorCallback_ = std::move(cb);
	}


	void enableRead()
	{
		events_ |= kReadEvent;
		update();
	}

	void disabelRead()
	{
		events_ &= ~kReadEvent;
		update();
	}

	void enableWrite()
	{
		events_ |= kWriteEvent;
		update();
	}

	void disableWrtie()
	{
		events_ &= ~kWriteEvent;
		update();
	}

	void disableAll()
	{
		events_ = kNoneEvent;
		update();
	}

	void handleEvent(Timestamp recieveTime);
private:
	EventLoop *loop_;
	const int fd_;

	//fd_在pollfd 数组中的索引, -1表示还未添加
	int index_;
	int events_;
	int revents_;

	ReadEventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_;
	EventCallback errorCallback_;

	static const int kReadEvent;
	static const int kWriteEvent;
	static const int kNoneEvent;

	void update();
};

#endif

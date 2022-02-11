#ifndef _NET_EVENTLOOP_H_
#define _NET_EVENTLOOP_H_


#include "base/noncopyable.h"
#include "base/Timestamp.h"

#include "Poller.h"
#include "PollPoller.h"
#include "EpollPoller.h"
#include "Channel.h"

#include <assert.h>
#include <stdint.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <vector>
#include <functional>
#include <memory>


//class EventLoop : public noncopyable
//{
//public:
//	EventLoop();
//
//	~EventLoop();
//	
//	void loop();
//	void quit();
//	void updateChannel(Channel *channel);
//	void removeChannel(Channel *channel);
//	bool hasChannel(Channel *channel);
//
//private:
//	using ChannelList = std::vector<Channel*>;
//
//	
//	Poller *poller_;
//	ChannelList activeChannelList_;
//	bool quit_;
//	bool running_;
//};



//增加eventfd用于唤醒poller_->poll
//#include<sys/eventfd.h>
//flags - EFD_CLOEXEC EFD_NONBLOCK
//int eventfd(unsigned int initval, int flags);
//返回一个文件描述符，内核只维护一个uint64变量
class EventLoop : public noncopyable
{
public:
	EventLoop();

	~EventLoop();
	
	void loop();
	void quit();
	void updateChannel(Channel *channel);
	void removeChannel(Channel *channel);
	bool hasChannel(Channel *channel);

private:
	using ChannelList = std::vector<Channel*>;

	
	int wakeupFd_;
	bool quit_;
	bool running_;

	std::unique_ptr<Poller> poller_;
	std::unique_ptr<Channel> wakeupChannel_;
	ChannelList activeChannelList_;

	void handleRead();
	void wakeup();
};



#endif 


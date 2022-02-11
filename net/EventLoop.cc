#include "EventLoop.h"

Poller *defaultPoller(EventLoop *loop)
{
	return new EpollPoller(loop);
}

int createEventfd()
{
	int efd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
	assert(efd >= 0);
	return efd;
}

EventLoop::EventLoop()
	:wakeupFd_(createEventfd())
	,quit_(false)
	,running_(false)
	,poller_(defaultPoller(this))
	,wakeupChannel_(new Channel(this, wakeupFd_))
{
	wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
	wakeupChannel_->enableRead();
}


void EventLoop::handleRead()
{
	uint64_t num;
	int n = ::read(wakeupFd_, &num, sizeof(num));
	assert(n == static_cast<int>(sizeof(num)));
}

void EventLoop::wakeup()
{
	uint64_t num = 0;
	int n = ::write(wakeupFd_,  &num, sizeof(num));
	assert(n == static_cast<int>(sizeof(num)));
}

EventLoop::~EventLoop()
{
	::close(wakeupFd_);
}


void EventLoop::loop()
{
	assert(!running_);
	assert(!quit_);
	running_ = true;
	
	while(!quit_)
	{
		activeChannelList_.clear();
		Timestamp ts = poller_->poll(&activeChannelList_);
		for(auto ch = activeChannelList_.cbegin(); ch != activeChannelList_.cend(); ++ch)
		{
			Channel *channel = *ch;
			channel->handleEvent(ts);	
		}
	}
}

void EventLoop::quit()
{
	quit_ = true;
	wakeup();
}

void EventLoop::updateChannel(Channel *channel)
{
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
	poller_->removeChannel(channel);	
}

bool EventLoop::hasChannel(Channel *channel)
{
	return poller_->hasChannel(channel);
}


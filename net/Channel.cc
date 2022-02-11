#include "Channel.h"

const int Channel::kReadEvent = POLLIN;
const int Channel::kWriteEvent = POLLOUT;
const int Channel::kNoneEvent = 0;

Channel::Channel(EventLoop *loop, int fd)
	:loop_(loop)
	,fd_(fd)
	,index_(-1)
{

}


void Channel::update()
{
	loop_->updateChannel(this);
}


void Channel::handleEvent(Timestamp recieveTime)
{
	//先做简单的测试
	if(revents_ & POLLIN)
	{
		if(readCallback_)
			readCallback_(recieveTime);
	}

	if(revents_ & POLLOUT)
	{
		if(writeCallback_)
			writeCallback_();
	}

	if(revents_ & (POLLERR | POLLNVAL))
	{
		if(errorCallback_)
			errorCallback_();
	}
}

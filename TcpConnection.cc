#include "TcpConnection.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"

#include <assert.h>

TcpConnection::TcpConnection(EventLoop *loop,
                            int sockfd,
                            const InetAddress &localAddr,
                            const InetAddress &peerAddr)
    :   loop_(loop),
        channel_(new Channel(loop, sockfd)),
        socket_(new Socket(sockfd)),
        state_(kConnecting),
        localAddr_(localAddr),
        peerAddr_(peerAddr)
{
    assert(loop != NULL);
    channel_->setRead
}
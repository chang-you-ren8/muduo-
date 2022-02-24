#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

//测试连接数量，以及多线程中的round-robin是否正确
void test(int n)
{
    struct sockaddr_in  serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(12345);

    char buf[1024];
    for(int i = 0; i < n; ++i)
    {
        int conn = socket(AF_INET, SOCK_STREAM, 0);
        assert(conn >= 0);
        int ret = connect(conn, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        assert(ret == 0);
        ssize_t n = ::read(conn, buf, sizeof(buf));
        if(n > 0)
        {
            buf[n] = '\0';
            printf("%s\n", buf);
        }
    }
}

int main(int argc, const char *argv[])
{
    assert(argc == 2);
    int n = atoi(argv[1]);
    //测试时发现TcpServer::removeConnection的connection_.erase重复删除了？
    test(n);
    return 0;
}
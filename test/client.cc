//用来测试EventLoop是否能支持多个连接

#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>


int main()
{
    int n = 50 *10000;

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET6, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(12345);

    for(int i = 0; i < n; ++i)
    {
        int conn = socket(AF_INET, SOCK_STREAM, 0);
        connect(conn, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    }

    return 0;
}
#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>

#define READ_BUFFER  255

Connection::Connection(EventLoop*eventloop,Socket *clntSock) :_eventloop(eventloop),_clntSock(clntSock) {
    int clnt_fd = clntSock->GetSocketFd();
    _clntChannel = new Channel(_eventloop,clnt_fd);
     std::function<void()> cb = std::bind(&Connection::echo, this, clnt_fd);
     _clntChannel->setHandleEvent(cb);
     _clntChannel->enableReading();
}

Connection:: ~Connection() {
    delete _clntSock;
    delete _clntChannel;
}
 void Connection::echo(int sockfd) {
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        memset(buf,0 ,READ_BUFFER);
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnectionCallback(_clntSock);
            break;
        }
    }
 }
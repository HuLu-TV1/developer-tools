#ifndef __CONNECTION__
#define __CONNECTION__
#include "noncopyable.h"
#include <functional>
class EventLoop;
class Socket;
class Channel;
class Connection :noncopyable{
    private:
    EventLoop *_eventloop;
    void echo(int sockfd);
    std::function<void(Socket*)> deleteConnectionCallback;
     Socket *_clntSock;
      Channel * _clntChannel ;
    public:
    Connection(EventLoop *eventloop,Socket *clntSock);
    ~Connection();
    void setDeleteConnectionCallback(std::function<void(Socket*)>cb) {deleteConnectionCallback = cb;}
};
#endif
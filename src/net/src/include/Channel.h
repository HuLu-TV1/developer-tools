#ifndef __CHANNEL__
#define __CHANNEL__
#include <sys/epoll.h>
#include <functional>
#include "noncopyable.h"

class Epoll;
class EventLoop;
class Channel : noncopyable {
 public:
  typedef std::function<void()> handleEventCallback;
  Channel(EventLoop *evloop, int fd);
  void enableReading();
  void setInEpoll();
  bool getInEpoll();
  int getEvents();
  void setRevents(int event);
  int getRevents();
  int getFd();
  void handleEvent();
  void setHandleEvent(handleEventCallback cb);

 private:
  int events;
  int revents;
  bool inEpoll;
  EventLoop *_evloop;
  int _fd;
  handleEventCallback handleEventcb;
};

#endif
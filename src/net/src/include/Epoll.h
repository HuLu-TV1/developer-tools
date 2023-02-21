#ifndef __EPOLL__
#define __EPOLL__

#include <sys/epoll.h>
#include <vector>
#include "noncopyable.h"

class Channel;
class Epoll : noncopyable {
 private:
  using EventVec = std::vector<struct epoll_event>;
  using ChannelVec = std::vector<Channel *>;
  void fillActiveChannels(int nfds, ChannelVec &vec_channel);
  EventVec events;
  int epfd;
  static const int kInitEventListSize = 16;
  ChannelVec vec_channel;

 public:
  Epoll();
  ~Epoll();
  std::vector<Channel *> createWait(int timeout = -1);
  void update(Channel *channel);
};

#endif
#ifndef __EPOLL__
#define __EPOLL__

#include "noncopyable.h"
#include <sys/epoll.h>
#include <vector>
namespace net {
class Channel;
class Epoll : noncopyable {
private:
  using EventVec = std::vector<struct epoll_event>;
  using ChannelVec = std::vector<Channel *>;
  void FillActiveChannels(int nfds, ChannelVec &vec_channel);
  EventVec events_;
  int epfd_;
  static const int kInitEventListSize = 16;
  ChannelVec vec_channel_;

public:
  Epoll();
  ~Epoll();
  std::vector<Channel *> CreateWait(int timeout = -1);
  void Update(Channel *channel);
};
} // namespace net
#endif
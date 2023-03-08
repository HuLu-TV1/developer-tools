#include "epoll.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "channel.h"
using namespace net;
const int kMaxEvents = 100;
Epoll::Epoll() : events_(kInitEventListSize) {
  epfd_ = epoll_create(kMaxEvents);
  if (epfd_ == -1) {
    printf("create epoll error: %s(errno: %d)\n", strerror(errno), errno);
  }
}

Epoll::~Epoll() {
  std::cout << "Epoll" << std::endl;
  close(epfd_);
}

void Epoll::Update(Channel *channel) {
  int fd = channel->get_fd();
  int event = channel->get_events();
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.data.ptr = channel;
  std::cout << "update fd " << fd << std::endl;
  ev.events = event;
  if (!channel->get_in_epoll()) {
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
      printf("epoll_ctl error: %s(errno: %d)\n", strerror(errno), errno);
    }
    channel->set_in_epoll();
  } else {
    if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
      printf("epoll_ctl error: %s(errno: %d)\n", strerror(errno), errno);
    }
  }
}

std::vector<Channel *> Epoll::CreateWait(int timeout) {
  ChannelVec vec_channel;
  int nfds = epoll_wait(epfd_, &*events_.begin(), static_cast<int>(events_.size()), timeout);
  std::cout << "nfds " << nfds << std::endl;
  FillActiveChannels(nfds, vec_channel);
  std::cout << " createWait vec_channel.size()" << vec_channel.size() << std::endl;
  // if(nfds == events.size()) {
  //     events.resize(events.size()*2);
  // }
  return vec_channel;
}
void Epoll::FillActiveChannels(int nfds, ChannelVec &vec_channel) {
  for (int i = 0; i < nfds; i++) {
    Channel *tem_channel = static_cast<Channel *>(events_[i].data.ptr);
    tem_channel->set_revents(events_[i].events);
    vec_channel.push_back(tem_channel);
    std::cout << " fillActive Channels vec_channel.size()" << vec_channel.size() << std::endl;
  }
}
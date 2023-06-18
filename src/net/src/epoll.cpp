#include "epoll.h"
#include "channel.h"
#include "log.h"
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
using namespace net;
const int kMaxEvents = 100;
Epoll::Epoll() : events_(kInitEventListSize) {
  epfd_ = epoll_create(kMaxEvents);
  if (epfd_ == -1) {
    LOG_ERROR("create epoll error: %s(errno: %d)\n", strerror(errno), errno);
  }
}

Epoll::~Epoll() {
  LOG_DEBUG("~Epoll");
  close(epfd_);
}

void Epoll::Update(Channel *channel) {
  int fd = channel->get_fd();
  int event = channel->get_events();
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.data.ptr = channel;
  LOG_DEBUG("update fd = %d", fd);
  ev.events = event;
  if (!channel->get_in_epoll()) {
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
      LOG_ERROR("epoll_ctl error: %s(errno: %d)\n", strerror(errno), errno);
    }
    channel->set_in_epoll();
  } else {
    if (channel->IsNoneEvent()) {
      if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, &ev) == -1) {
        LOG_ERROR("epoll_ctl error: %s(errno: %d)\n", strerror(errno), errno);
      }
    } else {
      if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
        LOG_ERROR("epoll_ctl error: %s(errno: %d)\n", strerror(errno), errno);
      }
    }
  }
}

std::vector<Channel *> Epoll::CreateWait(int timeout) {
  ChannelVec vec_channel;
  // int nfds = epoll_wait(epfd_, &*events_.begin(),
  // static_cast<int>(events_.size()), timeout); //∂ºø…“‘
  int nfds = epoll_wait(epfd_, events_.data(), static_cast<int>(events_.size()),
                        timeout);
  FillActiveChannels(nfds, vec_channel);
  LOG_DEBUG("createWait vec_channel.size() = %d", vec_channel.size());
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
  }
}
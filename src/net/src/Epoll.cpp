#include "Epoll.h"
#include <Channel.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#define MAX_EVENTS 100
Epoll::Epoll() : events(kInitEventListSize) {
  epfd = epoll_create(MAX_EVENTS);
  if (epfd == -1) {
    printf("create epoll error: %s(errno: %d)\n", strerror(errno), errno);
  }
}

Epoll::~Epoll() {
  std::cout << "Epoll" << std::endl;
  close(epfd);
}

void Epoll::update(Channel *channel) {
  int fd = channel->getFd();
  int event = channel->getEvents();
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.data.ptr = channel;
  std::cout << "update fd " << fd << std::endl;
  ev.events = event;
  if (!channel->getInEpoll()) {
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
      printf("epoll_ctl error: %s(errno: %d)\n", strerror(errno), errno);
    }
    channel->setInEpoll();
  } else {
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1) {
      printf("epoll_ctl error: %s(errno: %d)\n", strerror(errno), errno);
    }
  }
}

std::vector<Channel *> Epoll::createWait(int timeout) {
  ChannelVec vec_channel;
  int nfds = epoll_wait(epfd, &*events.begin(), static_cast<int>(events.size()), timeout);
  std::cout << "nfds " << nfds << std::endl;
  fillActiveChannels(nfds, vec_channel);
  std::cout << " createWait vec_channel.size()" << vec_channel.size() << std::endl;
  // if(nfds == events.size()) {
  //     events.resize(events.size()*2);
  // }
  return vec_channel;
}
void Epoll::fillActiveChannels(int nfds, ChannelVec &vec_channel) {
  for (int i = 0; i < nfds; i++) {
    Channel *tem_channel = static_cast<Channel *>(events[i].data.ptr);
    tem_channel->setRevents(events[i].events);
    vec_channel.push_back(tem_channel);
    std::cout << " fillActive Channels vec_channel.size()" << vec_channel.size() << std::endl;
  }
}
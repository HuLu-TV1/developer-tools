#ifndef _NET_SOCKETSOPS_H
#define _NET_SOCKETSOPS_H
#include "noncopyable.h"
#include <arpa/inet.h>
const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr);
template <typename To, typename From> inline To implicit_cast(From const &f) {
  return f;
}
int get_socket_error(int sockfd);
#endif
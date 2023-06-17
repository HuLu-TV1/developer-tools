#ifndef _INETADDRESS__
#define _INETADDRESS__

#include "noncopyable.h"
#include "sockets_ops.h"
#include <netinet/in.h>
#include <string>
namespace net {
class InetAddress : noncopyable {
public:
  InetAddress(int port, std::string addr);
  const struct sockaddr *get_sock_addr() const {
    return sockaddr_cast(&serveraddr_);
  };
  socklen_t get_addr_len() const {
    return static_cast<socklen_t>(sizeof(serveraddr_));
  }

private:
  struct sockaddr_in serveraddr_;
};
} // namespace net
#endif
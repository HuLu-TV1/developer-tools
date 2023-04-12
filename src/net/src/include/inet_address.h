#ifndef _INETADDRESS__
#define _INETADDRESS__

#include <netinet/in.h>
#include <string>
#include "noncopyable.h"
#include "sockets_ops.h"
namespace net {
class InetAddress : noncopyable {
 public:
  InetAddress(int port, std::string addr);
  const struct sockaddr *get_sock_addr() const { return sockaddr_cast(&serveraddr_); };

 private:
  struct sockaddr_in serveraddr_;
};
}  // namespace net
#endif
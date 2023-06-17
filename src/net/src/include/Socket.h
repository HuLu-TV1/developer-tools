#ifndef _SOCK__
#define _SOCK__
#include "noncopyable.h"
#include <sys/socket.h>

namespace net {
class InetAddress;
class Socket : noncopyable {
public:
  Socket(int fd);
  Socket();
  ~Socket();
  int CreateSocket(sa_family_t family);
  int get_socket_fd();
  void set_reuser_addr(int fd);
  void set_nonblocking(int fd);
  void Bind(int fd, const InetAddress &inetaddress);
  void connect(int fd, const InetAddress &inetaddress);
  void Listen(int fd, int num = 10);
  void shutdownWrite();
  int Accept(int fd /*,InetAddress & clientaddr*/);

private:
  int socket_fd;
};
} // namespace net

#endif
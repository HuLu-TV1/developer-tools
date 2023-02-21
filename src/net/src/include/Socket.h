#ifndef _SOCK__
#define _SOCK__
#include <sys/socket.h>
#include "noncopyable.h"
class InetAddress;

class Socket : noncopyable {
 public:
  Socket(int fd);
  Socket();
  ~Socket();
  int CreateSocket(sa_family_t family);
  int GetSocketFd();
  void SetReuserAddr(int fd);
  void SetNonBlocking(int fd);
  void Bind(int fd, const InetAddress &inetaddress);
  void Listen(int fd, int num = 10);
  int Accept(int fd /*,InetAddress & clientaddr*/);

 private:
  int socket_fd;
};
#endif
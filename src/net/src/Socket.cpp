

#include "Socket.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <cstring>
#include "InetAddress.h"

Socket::Socket() {}
Socket::~Socket() { close(socket_fd); }
int Socket::CreateSocket(sa_family_t family) {
  socket_fd = socket(family, SOCK_STREAM, 0);
  return socket_fd;
}
Socket::Socket(int fd) { socket_fd = fd; }
int Socket::GetSocketFd() { return socket_fd; }

void Socket::SetReuserAddr(int fd) {
  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Socket::SetNonBlocking(int fd) { fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK); }
void Socket::Bind(int fd, const InetAddress &inetaddress) {
  if (bind(fd, inetaddress.getSockAddrconst(), sizeof(inetaddress)) == -1) {
    printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
  }
}

void Socket::Listen(int fd, int num) {
  if (listen(fd, num) == -1) {
    printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
  }
}

int Socket::Accept(int fd /*,InetAddress & clientaddr*/) {
  // struct sockaddr_in clnt_adr;
  // socklen_t addrlen = static_cast<socklen_t>(sizeof(clnt_adr));
  // int clnt_fd = accept(fd,sockaddr_cast(&clnt_adr),&addrlen);
  int clnt_fd = accept(fd, (struct sockaddr *)NULL, NULL);
  return clnt_fd;
}

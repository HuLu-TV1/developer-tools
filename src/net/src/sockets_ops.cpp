#include "sockets_ops.h"
#include <errno.h>
const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr) {
  return static_cast<const struct sockaddr *>(
      implicit_cast<const void *>(addr));
}

int get_socket_error(int sockfd) {
  int optval;
  socklen_t optlen = static_cast<socklen_t>(sizeof optval);

  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
    return errno;
  } else {
    return optval;
  }
}
// struct sockaddr* sockaddr_cast(struct sockaddr_in*addr) {
//     return static_cast<const struct sockaddr*>(addr);
// }

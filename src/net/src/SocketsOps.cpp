#include "SocketsOps.h"

template <typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}

const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr) {
  return static_cast<const struct sockaddr *>(implicit_cast<const void *>(addr));
}

// struct sockaddr* sockaddr_cast(struct sockaddr_in*addr) {
//     return static_cast<const struct sockaddr*>(addr);
// }

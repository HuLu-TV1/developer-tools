#ifndef _INETADDRESS__
#define _INETADDRESS__

#include <netinet/in.h>
#include <string>
#include "noncopyable.h"
const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr);
template <typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}
class InetAddress : noncopyable {
 public:
  InetAddress(int port, std::string addr);
  const struct sockaddr *getSockAddrconst() const { return sockaddr_cast(&serveraddr); };

 private:
  struct sockaddr_in serveraddr;
};
#endif
#include "inet_address.h"
#include <arpa/inet.h>
using namespace net;
InetAddress ::InetAddress(int port, std::string addr) {
  serveraddr_.sin_family = AF_INET;
  serveraddr_.sin_addr.s_addr = inet_addr(addr.c_str());
  serveraddr_.sin_port = htons(port);
}
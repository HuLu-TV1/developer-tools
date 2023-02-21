#include "InetAddress.h"
#include <arpa/inet.h>
InetAddress ::InetAddress(int port, std::string addr) {
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(addr.c_str());
  serveraddr.sin_port = htons(port);
}
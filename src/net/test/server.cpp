#include "event_loop.h"
#include "inet_address.h"
#include "tcp_server.h"
#define BUF_SIZE 255
using namespace net;
int main() {
  InetAddress serv_addr(8888, "127.0.0.1");
  EventLoop eventloop;
  Server server(&eventloop, serv_addr);
  eventloop.loop();
  return 0;
}
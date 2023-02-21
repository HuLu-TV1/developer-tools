#include "EventLoop.h"
#include "InetAddress.h"
#include "Server.h"
#define BUF_SIZE 255

int main() {
  InetAddress serv_addr(8888, "127.0.0.1");
  EventLoop eventloop;
  Server server(&eventloop, serv_addr);
  eventloop.loop();
  return 0;
}
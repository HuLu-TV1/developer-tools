#ifndef _NET_SOCKETSOPS_H
#define _NET_SOCKETSOPS_H
#include <arpa/inet.h>
#include "noncopyable.h"
const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr);
#endif
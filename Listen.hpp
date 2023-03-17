#ifndef LISTEN_H
#define LISTEN_H

#include "Socket.hpp"
#include "Config.hpp"
#include <sys/types.h>
#include <poll.h>

void listen_clients(void);

#endif
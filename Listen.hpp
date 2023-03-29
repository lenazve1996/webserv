#ifndef LISTEN_H
#define LISTEN_H

#include <iostream>
#include "Socket.hpp"
#include "Config.hpp"
#include "ConfigParser.hpp"
#include <sys/types.h>
#include <fstream> 
#include <poll.h>
#include <string>
#include <sstream>

void listen_clients(ConfigParser parser);

#endif
#include "Config.hpp"

Config::Config( int port )
{
    _port = port;
}

Config::~Config() {}

int Config::getPort( void )
{
    return _port;
}
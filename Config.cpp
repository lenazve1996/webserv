#include "Config.hpp"

Config::Config( std::list<int> ports )
{
    _ports = ports;
}

Config::~Config() {}

std::list<int>  Config::getPorts( void )
{
    return _ports;
}
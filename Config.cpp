//
// Created by Larys Brianna on 3/15/23.
//

#include "Config.hpp"

Config::Config(): _host(std::string("0.0.0.0")), _serverName(std::vector<std::string>()), _errorPages(std::vector<std::vector<std::string> >()),   _root(std::string()),
                  _allowedMethods(std::vector<std::string>()), _index(std::vector<std::string>()),
                  _locations(std::vector<Location>()){
    this->_port = 80;
    this->_defaultServer = false;
    this->_autoindex = false;
    this->_bodySize = 1000000;
}

Config::~Config() {}
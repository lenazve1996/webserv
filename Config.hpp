//
// Created by Larys Brianna on 3/15/23.
//


#ifndef INFO_H
# define INFO_H

#include <iostream>
#include <vector>
#include "Location.hpp"

struct Config{
public:
    int		    _port;
    std::string _host;
    std::vector<std::string> _serverName;
    long long int         _bodySize;
    std::vector<std::vector<std::string> > _errorPages;
    std::string _root;
    std::vector<std::string>    _allowedMethods;
    std::vector<std::string>    _index;
    bool    _autoindex;
    std::vector<Location> _locations;
    bool 	_defaultServer;
};

//class Config {
//
//public:
//
//    Config();
//    ~Config();
//    Config(Config const &other);
//    Config operator=(Config const &other);
//
//    int		getPort( void );
//    std::string getHost(void);
//    int     getBodySize(void);
//
//private:
//
//    int		    _port;
//    std::string _host;
//    std::vector<std::string> _serverName;
//    int         _bodySize;
//    std::vector<std::vector<std::string> > _errorPages;
//    std::string _root;
//    std::vector<std::string>    _allowedMethods;
//    std::vector<std::string>    _index;
//    bool    _autoindex;
//	std::vector<Location> _locations;
//	bool 	_defaultServer;
//};

#endif
//
// Created by Larys Brianna on 3/21/23.
//

#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP

#include <iostream>
#include <vector>

struct Location{
public:
    std::string _root;
    std::vector<std::string>    _index;
    bool        _autoindex;
    std::vector<std::string>    _allowedMethods;
    std::vector<std::vector<std::string> >   _errorPages;
    std::string		_url;
    long long int _bodySize;
    std::vector<std::string>       _cgiPath;
    std::vector<std::string>        _cgiExt;

    Location();
    ~Location();
    Location(Location const& other);
    Location& operator=(Location const& other);
};

#endif //WEBSERV_LOCATION_HPP

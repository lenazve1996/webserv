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

    Location();
    ~Location();
    Location(Location const& other);
    Location& operator=(Location const& other);
};
//class Location {
//private:
//    std::string _root;
//    std::vector<std::string>    _index;
//    bool        _autoindex;
//    std::vector<std::string>    _allowedMethods;
//    std::vector<std::vector<std::string> >   _errorPages;
//	std::string		_url;
//public:
//    Location();
//    ~Location();
//    Location(Location const &other);
//    Location &operator=(Location const &other);
//
//
//    void setRoot(const std::string &root);
//    const std::string &getRoot() const;
//
//    void setIndex(const std::vector<std::string> &index);
//    const std::vector<std::string> &getIndex() const;
//
//    void setAutoIndex(const bool &autoindex);
//    const bool &getAutoIndex() const;
//
//    void setAllowedMethods(const std::vector<std::string> &methods);
//    const std::vector<std::string> &getAllowedMethods() const;
//
//    void setErrorPages(const std::vector<std::vector<std::string> > &page);
//    const std::vector<std::vector<std::string> > &getErrorPages() const;
//};


#endif //WEBSERV_LOCATION_HPP

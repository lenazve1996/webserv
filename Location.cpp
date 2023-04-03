//
// Created by Larys Brianna on 3/21/23.
//

#include "Location.hpp"

Location::Location() {
    this->_allowedMethods = std::vector<std::string>();
    this->_root = "";
    this->_index = std::vector<std::string>();
    this->_cgiExt = std::vector<std::string>();
    this->_cgiPath = std::vector<std::string>();
    this->_url = std::string();
    this->_autoindex = false;
    this->_errorPages = std::vector< std::vector<std::string> >();
}

Location::~Location() {}
Location::Location(const Location &other) : _root(other._root), _index(other._index), _autoindex(other._autoindex), _allowedMethods(other._allowedMethods), _errorPages(other._errorPages), _url(other._url), _cgiPath(other._cgiPath), _cgiExt(other._cgiExt){}

Location &Location::operator=(const Location &other) {
    if (this!=&other){
        _root = other._root;
        _index = other._index;
        _autoindex = other._autoindex;
        _allowedMethods = other._allowedMethods;
        _errorPages = other._errorPages;
        _url = other._url;
        _cgiPath = other._cgiPath;
        _cgiExt = other._cgiExt;
    }
    return *this;
}

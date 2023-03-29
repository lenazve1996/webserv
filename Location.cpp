//
// Created by Larys Brianna on 3/21/23.
//

#include "Location.hpp"

Location::Location() {
    this->_allowedMethods = std::vector<std::string>();
    this->_root = "";
    this->_index = std::vector<std::string>();
    this->_autoindex = false;
    this->_errorPages = std::vector< std::vector<std::string> >();
}

Location::~Location() {}
Location::Location(const Location &other) : _root(other._root), _index(other._index), _autoindex(other._autoindex), _allowedMethods(other._allowedMethods), _errorPages(other._errorPages), _url(other._url){}

Location &Location::operator=(const Location &other) {
    if (this!=&other){
        _root = other._root;
        _index = other._index;
        _autoindex = other._autoindex;
        _allowedMethods = other._allowedMethods;
        _errorPages = other._errorPages;
        _url = other._url;
    }
    return *this;
}

//Location::Location(const Location &other) {
//    this->_root = other._root;
//    this->_errorPages = other._errorPages;
//    this->_index = other._index;
//    this->_autoindex = other._autoindex;
//    this->_allowedMethods = other._allowedMethods;
//}
//
//Location &Location::operator=(const Location &other) {
//    *this = Location(other);
//    return *this;
//}
//
//void Location::setRoot(const std::string &root) {
//    _root = root;
//}
//
//const std::string &Location::getRoot() const {
//    return _root;
//}
//
//void Location::setAllowedMethods(const std::vector <std::string> &methods) {
//    _allowedMethods = methods;
//}
//
//const std::vector<std::string> &Location::getAllowedMethods() const {
//    return _allowedMethods;
//}
//
//void Location::setAutoIndex(const bool &autoindex) {
//    _autoindex = autoindex;
//}
//
//const bool &Location::getAutoIndex() const {
//    return _autoindex;
//}
//
//void Location::setIndex(const std::vector<std::string> &index) {
//    _index = index;
//}
//
//const std::vector<std::string> &Location::getIndex() const {
//    return _index;
//}

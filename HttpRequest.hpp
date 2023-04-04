#ifndef HTTP_REQEST_HPP
#define HTTP_REQEST_HPP

#include "Config.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

class HttpRequest {
private:
  std::string _request;
  std::string _requestType;
  std::string _URI;
  std::string _protocol;
  std::string _host;
  std::string _body;
  int _fd;

  // void parseMethod();
  void parseStartString(const std::string &header);
  void parseHeaders(const std::vector<std::string> &splittedRequest);
  void parseBody(const std::vector<std::string> &splittedRequest);


  void print();

public:
  //   HttpRequest();
  HttpRequest(int fd);
  HttpRequest(HttpRequest &otherRequest);
  ~HttpRequest();
  HttpRequest &operator=(const HttpRequest &otherRequest);
  void reset(int fd);
  void readRequest();
  void parse();

  int checkStartString(Config &config);
  int checkHost(Config &config);

  std::string getRequest() const;
  std::string getRequestType() const;
  std::string getURI() const;
  std::string getProtocol() const;
  std::string getHost() const;
  std::string getBody() const;
  int getFd() const;
};

#endif

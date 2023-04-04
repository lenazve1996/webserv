#include "./HttpRequest.hpp"

HttpRequest::HttpRequest(int fd) {
  _request = "";
  _requestType = "";
  _URI = "";
  _protocol = "";
  _host = "";
  _body = "";
  _fd = fd;
}

HttpRequest::HttpRequest(HttpRequest &otherRequest) { *this = otherRequest; }

void HttpRequest::reset(int fd) {
  this->_fd = fd;
  this->_request.clear();
  this->_requestType.clear();
  this->_URI.clear();
  this->_protocol.clear();
  this->_host.clear();
  this->_body.clear();
}

void HttpRequest::readRequest() {
  char strBuf[2048];
  int num;

  if (_fd != -1) {
    num = 0;
    num = read(_fd, &strBuf, 2047);
    strBuf[num] = 0;
    _request += strBuf;
    if (num == 2047)
      this->readRequest();
  }
}

static std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);

  while (std::getline(tokenStream, token, delimiter))
    tokens.push_back(token);
  return tokens;
}

static bool ft_isspace(char ch) {
  if (ch == '\f' || ch == '\v' || ch == '\n' || ch == '\t' || ch == ' ' ||
      ch == '\r')
    return (true);
  return (false);
}

/*void HttpRequest::parseMethod() {
  size_t counter;

  counter = 0;
  while (ft_isspace(_request[counter]))
    counter++;
  if (_request[counter] &&
      (_request[counter] == 'G' || _request[counter] == 'g') &&
      _request[counter + 1] &&
      (_request[counter + 1] == 'E' || _request[counter + 1] == 'e') &&
      _request[counter + 2] &&
      (_request[counter + 2] == 'T' || _request[counter + 2] == 't') &&
      _request[counter + 3] && (_request[counter + 3] == ' '))
    _requestType = "GET";
  else if (_request[counter] &&
           (_request[counter] == 'P' || _request[counter] == 'p') &&
           _request[counter + 1] &&
           (_request[counter + 1] == 'O' || _request[counter + 1] == 'o') &&
           _request[counter + 2] &&
           (_request[counter + 2] == 'S' || _request[counter + 2] == 's') &&
           _request[counter + 3] &&
           (_request[counter + 3] == 'T' || _request[counter + 3] == 't') &&
           _request[counter + 4] && (_request[counter + 4] == ' '))
    _requestType = "POST";
  else if (_request[counter] &&
           (_request[counter] == 'D' || _request[counter] == 'd') &&
           _request[counter + 1] &&
           (_request[counter + 1] == 'E' || _request[counter + 1] == 'e') &&
           _request[counter + 2] &&
           (_request[counter + 2] == 'L' || _request[counter + 2] == 'l') &&
           _request[counter + 3] &&
           (_request[counter + 3] == 'E' || _request[counter + 3] == 'e') &&
           _request[counter + 3] &&
           (_request[counter + 4] == 'T' || _request[counter + 4] == 't') &&
           _request[counter + 3] &&
           (_request[counter + 5] == 'E' || _request[counter + 5] == 'e') &&
           _request[counter + 4] && (_request[counter + 6] == ' '))
    _requestType = "DELETE";
  else
    _requestType = "UNKNOWN";
}*/

void HttpRequest::parseStartString(const std::string &header) {
  std::vector<std::string> splittedHeader = split(header, ' ');
  _requestType = splittedHeader[0];
  _URI = splittedHeader[1];
  _protocol = splittedHeader[2];
}

void HttpRequest::parseHeaders(
    const std::vector<std::string> &splittedRequest) {
  size_t i;
  size_t j;

  i = 0;
  while (splittedRequest[i++] != "\n") {
    if (splittedRequest[i].rfind("Host:", 0) == 0) {
      j = 3;
      while (splittedRequest[i][j] && splittedRequest[i][j] != ':')
        j++;
      while (splittedRequest[i][j] && ft_isspace(splittedRequest[i][j]))
        j++;
      _host = splittedRequest[i][j];
    }
  }
}

void HttpRequest::parseBody(const std::vector<std::string> &splittedRequest) {
  std::vector<std::string>::const_iterator it;
  _body = "";

  it = splittedRequest.begin();
  while (*it != "\n") {
    it++;
  }
  while (it != splittedRequest.end()) {
    _body += *it;
  }
}

void HttpRequest::parse() {
  std::vector<std::string> splittedRequest = split(_request, '\n');
  parseStartString(splittedRequest[0]);
  parseHeaders(splittedRequest);
  parseBody(splittedRequest);
  _request.clear();
  print();
}

int HttpRequest::checkStartString(Config &config) {
  if (std::find(config._allowedMethods.begin(), config._allowedMethods.end(),
                _requestType) == config._allowedMethods.end())
    return (405);
  if (_protocol != "HTTP/1.1\n")
    return (400);
  return (200);
}

int HttpRequest::checkHost(Config &config) {
  if (std::find(config._serverName.begin(), config._serverName.end(),
                this->_host) == config._serverName.end())
    return (404);
  return (200);
}

HttpRequest &HttpRequest::operator=(const HttpRequest &otherRequest) {
  this->_request = otherRequest.getRequest();
  this->_fd = otherRequest.getFd();
  this->_requestType = otherRequest.getRequestType();
  this->_URI = otherRequest.getURI();
  this->_protocol = otherRequest.getProtocol();
  this->_host = otherRequest.getHost();
  this->_body = otherRequest.getBody();
  return (*this);
}

void HttpRequest::print() {
  std::cout << this->getRequest() << std::endl;
  std::cout << this->getRequestType() << std::endl;
  std::cout << this->getURI() << std::endl;
  std::cout << this->getProtocol() << std::endl;
  std::cout << this->getHost() << std::endl;
  std::cout << this->getBody() << std::endl;
}

std::string HttpRequest::getRequest() const { return (_request); }
std::string HttpRequest::getRequestType() const { return (_requestType); }
std::string HttpRequest::getURI() const { return (_URI); }
std::string HttpRequest::getProtocol() const { return (_protocol); }
std::string HttpRequest::getHost() const { return (_host); }
std::string HttpRequest::getBody() const { return (_body); }
int HttpRequest::getFd() const { return (_fd); }

HttpRequest::~HttpRequest() {}

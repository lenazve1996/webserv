//
// Created by Larys Brianna on 3/20/23.
//

#include "ConfigParser.hpp"
#include <sstream>

ConfigParser::ConfigParser() {}

ConfigParser::ConfigParser(const ConfigParser &other) { *this = other; }

ConfigParser::~ConfigParser() {}

ConfigParser &ConfigParser::operator=(const ConfigParser &other) {
  if (this != &other) {
    this->_confStr = other._confStr;
    this->_serversArray = other._serversArray;
    this->_servers = other._servers;
    this->_numServs = other._numServs;
  }
  return *this;
}

void ConfigParser::_processConfig(std::string argv) {
  try {
    if ((argv.size() - strlen(".conf") < 0) ||
		argv.substr((argv.size() - strlen(".conf")), strlen(".conf")) !=
        ".conf")
      throw std::invalid_argument("Wrong file extension");
    else {
      this->_configFile.open(argv, std::ios_base::in);
      if (!this->_configFile.is_open())
        throw std::runtime_error("Couldn't open the config");
      this->processFile();
      this->_numServs = this->_servers.size();
      for (int i = 0; i < this->_numServs; i++) {
        this->_serversArray.push_back(Config());
        this->processBlocks(this->_servers[i], i);
      }
      if (!this->isEnoughInfo())
        throw std::runtime_error(
            "Not enough info provided in configuration file");
    }
  } catch (std::exception &e) {
	std::cerr << e.what() << std::endl;
	exit(0);
  }
}

bool ConfigParser::isEnoughInfo() {
  for (std::size_t i = 0; i < this->_serversArray.size(); i++) {
    if (this->_serversArray[i]._host.empty() &&
        this->_serversArray[i]._serverName.empty())
      return false;
    if (this->_serversArray[i]._locations.empty())
      return false;
    for (std::size_t j = 0; j < this->_serversArray[i]._locations.size(); j++) {
      if (this->_serversArray[i]._locations[j]._root.empty() &&
          this->_serversArray[i]._locations[j]._index.empty())
        return false;
    }
  }
  return true;
}

void ConfigParser::processFile() {
  if (this->_configFile.peek() == std::ifstream::traits_type::eof())
    throw std::runtime_error("Config is empty");
  std::stringstream buff;
  buff << this->_configFile.rdbuf();
  this->_confStr = buff.str();
  this->_configFile.close();
  if (!this->checkBlocks())
    throw std::runtime_error("Wrong number of brackets");
  this->iterateStr();
}

bool ConfigParser::checkBlocks() {
  int open = 0;
  int close = 0;
  bool first;
  std::size_t i = 0;

  while (i < this->_confStr.size()) {
    if (!first && this->_confStr[i] == '{')
      first = true;
    if (!first && this->_confStr[i] == '}')
      return (false);
    if (this->_confStr[i] == '{')
      open++;
    if (this->_confStr[i] == '}')
      close++;
    i++;
  }
  if (open > 0 && (open == close))
    return (true);
  else
    return false;
}

////extract every server block && check if it is correct
void ConfigParser::iterateStr() {
  std::string block;
  std::size_t i = 0;

  while (i < this->_confStr.size()) {
    if (!std::isspace(this->_confStr[i]) && this->_confStr[i] != 's')
      throw std::runtime_error("Only server blocks are allowed[1]");
    if (this->_confStr[i] == 's') {
      if (!this->_confStr.compare(i, 6, "server")) {
        i += 6;
        while (this->_confStr[i] != '{') {
          if (!std::isspace(this->_confStr[i]))
            throw std::runtime_error("Wrong server block synthax");
          i++;
        }
        i += splitBlocks(i) - 1;
      } else
        throw std::runtime_error("Only server block are allowed[2]");
    }
    i++;
  }
}

int ConfigParser::splitBlocks(int i) {
  std::string block;
  int size;

  block = this->extractBrackets(this->_confStr.substr(i));
  if (!this->isServerValid(block))
    throw std::runtime_error("Invalid server block");
  size = block.size();
  block = block.substr(1, block.size() - 2);
  if (!this->checkBrackets(block))
    throw std::runtime_error("Empty brackets in server block");
  this->_servers.push_back(block);
  return (size);
}

std::string ConfigParser::extractBrackets(std::string text) {
  std::size_t i = 1;
  std::size_t open = 1;
  std::size_t close = 0;

  while (i < text.size()) {
    if (text[i] == '{')
      open++;
    if (text[i] == '}') {
      close++;
      if (open == close)
        return (text.substr(0, i + 1));
    }
    i++;
  }
  throw std::runtime_error("Brackets aren't closed");
}

bool ConfigParser::isServerValid(std::string text) {
  int open = 0;
  int close = 0;
  std::size_t i = 0;

  while (i < text.size()) {
    if (text[i] == '{')
      open++;
    if (text[i] == '}')
      close++;
    i++;
  }
  if (open == close && open > 0)
    return true;
  else
    return false;
}

bool ConfigParser::checkBrackets(std::string text) {
  std::size_t i = 0;
  std::size_t nonSpace = 0;
  std::string substr;
  while (i < text.size()) {
    if (!std::isspace(text[i]) && text[i] != '{')
      nonSpace++;
    if (text[i] == '{' && !nonSpace)
      return false;
    else if (text[i] == '{' && nonSpace) {
      substr = this->extractBrackets(text.substr(i));
      substr = substr.substr(1, substr.size() - 2);
      return this->checkBrackets(substr);
    }
    i++;
  }
  if (!nonSpace)
    return false;
  else
    return true;
}
//
//// str27
void ConfigParser::processBlocks(std::string block, int server_index,
                                 bool is_loc) {
  std::size_t i = 0;
  int dir_type;
  std::string type;

  while (i < block.size()) {
    if (block[i] == '#') {
      int len = 0;
      len = block.substr(i, block.substr(i).find_first_of("\n")).size();
      i += len;
    }
    if (std::isalnum(block[i])) {
      type = block.substr(i, block.substr(i).find_first_of(" \t\n"));
      //            dir_type = this->directiveTypes(block.substr(i,
      //            block.substr(i).find_first_of(" \t\n")));
      dir_type = this->directiveTypes(type);
      if (dir_type == BAD_INSTRUCTIONS)
        throw std::runtime_error("Invalid instructions provided");
      if (dir_type == LOCATION && is_loc == true)
        throw std::runtime_error("Invalid location");
      else
        i += this->processInstructions(dir_type, block.substr(i), server_index,
                                       is_loc);
    }
    i++;
  }
}

int ConfigParser::directiveTypes(std::string dir) {
  if (!dir.compare("location"))
    return (LOCATION);
  else if (!dir.compare("listen"))
    return (LISTEN);
  else if (!dir.compare("root"))
    return (ROOT);
  else if (!dir.compare("server_name"))
    return (SERVER_NAME);
  else if (!dir.compare("error_page"))
    return (ERR_PAGE);
  else if (!dir.compare("client_max_body_size"))
    return (CLIENT_BODY_SIZE);
  else if (!dir.compare("allow_methods"))
    return (ALLOW_METHODS);
  else if (!dir.compare("autoindex"))
    return (AUTOINDEX);
  else if (!dir.compare("index"))
    return (INDEX);
  else
    return (BAD_INSTRUCTIONS);
}

int ConfigParser::processInstructions(int type, std::string dir, int ind,
                                      bool is_loc) {
  std::size_t args;
  std::size_t dir_len;

  if (type == LOCATION)
    return (this->processLocation(dir, ind));
  if (!this->isDirectiveValid(dir))
    throw std::runtime_error("Invalid direcitive");
  args = extractArgs(dir, ';').size();
  dir_len = dir.substr(0, dir.find(";")).size() + 1;
  switch (type) {
  case (LISTEN):
    this->processListen(dir, ind, args, is_loc);
    break;
  case (ROOT):
    this->processRoot(dir, ind, args, is_loc);
    break;
  case (SERVER_NAME):
    this->processServerName(dir, ind, args, is_loc);
    break;
  case (ERR_PAGE):
    this->processErrorPage(dir, ind, args, is_loc);
    break;
  case (CLIENT_BODY_SIZE):
    this->processBodySize(dir, ind, args, is_loc);
    break;
  case (AUTOINDEX):
    this->processAutoindex(dir, ind, args, is_loc);
    break;
  case (INDEX):
    this->processIndex(dir, ind, args, is_loc);
    break;
  case (ALLOW_METHODS):
    this->processAllow(dir, ind, args, is_loc);
    break;
    //		default:
    //			throw std::runtime_error("Invalid instructions");
    //			break;
  }
  return (dir_len);
}

std::size_t ConfigParser::processLocation(std::string dir, int ind) {
  std::string block;
  std::size_t beg;
  std::size_t end;
  Location loc;
  std::vector<std::string> args;

  if (!this->isLocationValid(dir))
    throw std::runtime_error("Wrong location block");
  beg = dir.find("{");
  end = dir.find("}");
  block = dir.substr(beg + 1, end - beg - 1);
  this->_serversArray[ind]._locations.push_back(loc);
  args = this->extractArgs(dir, '{');
  if (args.size() != 2)
    throw std::runtime_error("Invalid location block");
  this->_serversArray[ind]
      ._locations[this->_serversArray[ind]._locations.size() - 1]
      ._url = args[1];
  this->copyValues(
      this->_serversArray[ind]
          ._locations[this->_serversArray[ind]._locations.size() - 1],
      ind);
  this->processBlocks(block, ind, true);
  return (end + 1);
}

void ConfigParser::copyValues(Location &loc, int serv_ind) {
  loc._root = this->_serversArray[serv_ind]._root;
  loc._index = this->_serversArray[serv_ind]._index;
  loc._autoindex = this->_serversArray[serv_ind]._autoindex;
  loc._allowedMethods = this->_serversArray[serv_ind]._allowedMethods;
  loc._errorPages = this->_serversArray[serv_ind]._errorPages;
  // add body size???
}

bool ConfigParser::isLocationValid(std::string dir) {
  std::size_t i = 0;
  std::size_t start;
  while (std::isspace(dir[i]) && i < dir.size())
    i++;
  if (i == dir.size() || dir.substr(i).find("{") == std::string::npos ||
      this->extractArgs(dir.substr(i), '{').size() != 2)
    return false;
  start = dir.substr(i).find("{");
  if (start == dir.size())
    return false;
  start++;
  if (dir.substr(start).find("}") == std::string::npos)
    return false;
  return true;
}

bool ConfigParser::isDirectiveValid(std::string dir) {
  if (dir.find(";") == std::string::npos)
    return false;
  if (this->extractArgs(dir, ';').size() < 2)
    return false;
  else
    return true;
}

std::vector<std::string> ConfigParser::extractArgs(std::string dir,
                                                   char delim) {
  std::vector<std::string> args;
  int i = 0;
  int size_args = 0;

  while (dir[i] != delim) {
    while (std::isspace(dir[i]))
      i++;
    while (!std::isspace(dir[i + size_args]) && dir[i + size_args] != delim)
      size_args++;
    if (size_args > 1 || (size_args == 1 && dir[i + size_args] != delim))
      args.push_back(dir.substr(i, size_args));
    else
      break;
    i += size_args;
    size_args = 0;
  }
  return (args);
}
//
////process directives

void ConfigParser::processListen(std::string dir, int ind, int arg_num,
                                 bool is_loc) {
  std::vector<std::string> args;
  int port;
  std::string port_str;

  if (is_loc)
    throw std::runtime_error("No listen directive in location block");
  if (arg_num < 2 || arg_num > 3)
    throw std::runtime_error("Wrong number of arguments");
  args = this->extractArgs(dir, ';');
  if (args[1].find(":") != std::string::npos) {
    if (!this->checkIP(args[1].substr(0, args[1].find(":"))))
      throw std::runtime_error("Invalid ip address");
    this->_serversArray[ind]._host = args[1].substr(0, args[1].find(":"));
    port_str = args[1].substr(args[1].find(":") + 1);
  } else
    port_str = args[1];
  if (port_str.size() > 5)
    throw std::runtime_error("Invalid port number");
  for (std::size_t i = 0; i < port_str.size(); i++) {
    if (!std::isdigit(port_str[i]))
      throw std::runtime_error("Invalid port number"); // max port num is 65535
  }
  port = atoi(port_str.c_str());
  if (port > 65535)
    throw std::runtime_error("Invalid port number");
  this->_serversArray[ind]._port = port;
  if (arg_num == 3) {
    if (args[2].compare("default_server"))
      throw std::runtime_error("Third argument for default_server only");
    this->_serversArray[ind]._defaultServer = true;
  }
}

bool ConfigParser::checkIP(std::string ip) {
  std::string delim = ".";
  std::string token;
  std::size_t i = 0;
  bool stop = false;

  if (!ip.compare("localhost"))
    return true;
  while (!stop) {
    if ((i = ip.find(delim)) == std::string::npos)
      stop = true;
    token = ip.substr(0, i);
    if (token.size() > 3)
      return false;
    for (std::size_t j = 0; j < token.size(); j++) {
      if (!std::isdigit(token[j]))
        return false;
    }
    if (atoi(token.c_str()) > 255)
      return false;
    ip.erase(0, i + delim.length());
  }
  return true;
}

void ConfigParser::processRoot(std::string dir, int ind, int arg_num,
                               bool is_loc) {
  std::vector<std::string> arg;

  if (arg_num != 2)
    throw std::runtime_error("One argument for root must be provided");
  arg = this->extractArgs(dir, ';');
  if (is_loc && !this->_serversArray[ind]
                     ._locations[this->_serversArray[ind]._locations.size() - 1]
                     ._root.empty())
    this->_serversArray[ind]
        ._locations[this->_serversArray[ind]._locations.size() - 1]
        ._root.clear();
  else if (!this->_serversArray[ind]._root.empty())
    this->_serversArray[ind]._root.clear();
  if (is_loc)
    this->_serversArray[ind]
        ._locations[this->_serversArray[ind]._locations.size() - 1]
        ._root = arg[1];
  else
    this->_serversArray[ind]._root = arg[1];
}

void ConfigParser::processErrorPage(std::string dir, int ind, int arg_num,
                                    bool is_loc) {
  std::vector<std::string> args;
  std::vector<std::string> err;

  if (arg_num < 3)
    throw std::runtime_error(
        "At least two arguments for error_page must be provided");
  args = this->extractArgs(dir, ';');
  for (std::size_t i = 1; i < args.size(); i++)
    err.push_back(args[i]);
  if (is_loc)
    this->_serversArray[ind]
        ._locations[this->_serversArray[ind]._locations.size() - 1]
        ._errorPages.push_back(err);
  else
    this->_serversArray[ind]._errorPages.push_back(err);
}

void ConfigParser::processBodySize(std::string dir, int ind, int arg_num,
                                   bool is_loc) {
  std::vector<std::string> args;
  int mult = 1;
  int body_size_int;

  if (arg_num != 2)
    throw std::runtime_error(
        "Two arguments for client_body_size must be provided");
  args = this->extractArgs(dir, ';');
  for (std::size_t i = 0; i < args[1].size(); i++) {
    if (!std::isdigit(args[1][i]) && i == args[1].size() - 1)
      throw std::runtime_error("Invalid syntax for body_size");
    else if (!std::isdigit(args[1][i]) && i == args[1].size() - 1) {
      if (args[1][args.size() - 1] == 'M' || args[1][args.size() - 1] == 'm') {
        mult = 1000000;
        args[1] = args[1].substr(0, args[1].size() - 1);
      } else
        throw std::runtime_error("Invalid syntax in client_body_size");
    }
  }
  long long int body_size = std::strtol(args[1].c_str(), NULL, 10);
  if (body_size == LONG_MAX)
    throw std::runtime_error("client_body_size is too long");
  if (body_size > (std::numeric_limits<int>::max() / mult))
    throw std::runtime_error("wrong client_body_size");
  body_size_int = static_cast<int>(body_size);
  body_size_int *= mult;

  if (is_loc)
    this->_serversArray[ind]
        ._locations[this->_serversArray[ind]._locations.size() - 1]
        ._bodySize = body_size_int;
  this->_serversArray[ind]._bodySize = body_size_int;
}

void ConfigParser::processAutoindex(std::string dir, int ind, int arg_num,
                                    bool is_loc) {
  std::vector<std::string> args;
  bool flag = false;

  if (arg_num != 2)
    throw std::runtime_error("Invalid autoindex argument");
  args = this->extractArgs(dir, ';');
  if (!args[1].compare("on"))
    flag = true;
  if (is_loc)
    this->_serversArray[ind]
        ._locations[this->_serversArray[ind]._locations.size() - 1]
        ._autoindex = flag;
  else
    this->_serversArray[ind]._autoindex = flag;
}

void ConfigParser::processIndex(std::string dir, int ind, int arg_num,
                                bool is_loc) {
  std::vector<std::string> args;
  std::vector<std::string> values;

  if (arg_num < 2)
    throw std::runtime_error("Wrong number of index aruments");
  args = this->extractArgs(dir, ';');
  for (std::size_t i = 1; i < args.size(); i++)
    values.push_back(args[i]);

  if (is_loc && !this->_serversArray[ind]
                     ._locations[this->_serversArray[ind]._locations.size() - 1]
                     ._index.empty())
    this->_serversArray[ind]
        ._locations[this->_serversArray[ind]._locations.size() - 1]
        ._index.clear();
  else if (this->_serversArray[ind]._index.empty())
    this->_serversArray[ind]._index.clear();
  if (is_loc)
    this->_serversArray[ind]
        ._locations[this->_serversArray[ind]._locations.size() - 1]
        ._index = values;
  else
    this->_serversArray[ind]._index = values;
}

void ConfigParser::processAllow(std::string dir, int ind, int arg_num,
                                bool is_loc) {
  std::vector<std::string> args;

  if (arg_num < 2)
    throw std::runtime_error("Wrong number of aruments in allow_methods");
  args = this->extractArgs(dir, ';');
  if (is_loc && !this->_serversArray[ind]
                     ._locations[this->_serversArray[ind]._locations.size() - 1]
                     ._allowedMethods.empty())
    this->_serversArray[ind]
        ._locations[this->_serversArray[ind]._locations.size() - 1]
        ._allowedMethods.clear();
  else if (!this->_serversArray[ind]._allowedMethods.empty())
    this->_serversArray[ind]._allowedMethods.clear();

  for (std::size_t i = 1; i < args.size(); i++) {
    if (args[i].compare("GET") && args[i].compare("POST")) // TODO DELETE???
      throw std::runtime_error(
          "Wrong type of expression provided for allowed_methods");
    if (is_loc)
      this->_serversArray[ind]
          ._locations[this->_serversArray[ind]._locations.size() - 1]
          ._allowedMethods.push_back(args[i]);
    else
      this->_serversArray[ind]._allowedMethods.push_back(args[i]);
  }
}

void ConfigParser::processServerName(std::string dir, int ind, int arg_num,
                                     bool is_loc) {
  std::vector<std::string> args;

  if (arg_num < 2)
    throw std::runtime_error(
        "Wrong amount of arguments in server_name directive");
  if (is_loc)
    throw std::runtime_error("No server_name directive in location block");
  args = this->extractArgs(dir, ';');
  for (std::size_t i = 1; i < args.size(); i++) {
    if (args[i][0] == '"' && args[i][args[i].size() - 1] == '"')
      args[i] = args[i].substr(1, args[i].size() - 2);
  }
  if (!this->_serversArray[ind]._serverName.empty())
    this->_serversArray[ind]._serverName.clear();
  for (std::size_t i = 1; i < args.size(); i++) {
    this->_serversArray[ind]._serverName.push_back(args[i]);
  }
}

std::vector<Config> ConfigParser::getServersArray() {
  return this->_serversArray;
}

int ConfigParser::getNumServs() { return this->_numServs; }
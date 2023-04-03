//
// Created by Larys Brianna on 3/20/23.
//

#ifndef WEBSERV_CONFIGPARSER_HPP
#define WEBSERV_CONFIGPARSER_HPP

#include <vector>
#include <sstream>
#include <fstream>
#include "Config.hpp"
#include "Location.hpp"

#define LOCATION 1
#define LISTEN 2
#define ROOT 3
#define SERVER_NAME	4
#define ERR_PAGE	5
#define CLIENT_BODY_SIZE	6
#define ALLOW_METHODS 7
#define AUTOINDEX	8
#define INDEX	9
#define CGI     10
#define BAD_INSTRUCTIONS	0

class ConfigParser {
public:
	typedef std::vector<Config>	servers_array;
private:
    std::vector<std::string> _servers;
    int     _numServs;
    std::ifstream   _configFile;
    std::string     _confStr;
	servers_array _serversArray;
public:
    ConfigParser();
    ~ConfigParser();
    ConfigParser(ConfigParser const &other);
    ConfigParser& operator=(const ConfigParser& other);

    void _processConfig(std::string argv);
    void processFile();
    bool    checkBlocks();
    void    iterateStr();
    int splitBlocks(int i);
    std::string extractBrackets(std::string text);
    bool isServerValid(std::string text);
    bool    checkBrackets(std::string text);
    std::vector<std::string> extractArgs(std::string dir, char delim);

    bool isEnoughInfo();

	void    processBlocks(std::string block, int server_index, bool is_loc=false);
	int directiveTypes(std::string dir);
	int processInstructions(int type, std::string dir, int ind, bool is_loc);
	std::size_t processLocation(std::string dir, int ind);
	bool isDirectiveValid(std::string dir);

	bool isLocationValid(std::string loc);
	void copyValues(Location& loc, int serv_ind);

	//process directives
	void processListen(std::string dir, int ind, int arg_num, bool is_loc=false);
	void processRoot(std::string dir, int ind, int arg_num, bool is_loc=false);
	void processServerName(std::string dir, int ind, int arg_num, bool is_loc=false);
	void processErrorPage(std::string dir, int ind, int arg_num, bool is_loc=false);
	void processBodySize(std::string dir, int ind, int arg_num, bool is_loc=false);
	void processAllow(std::string dir, int ind, int arg_num, bool is_loc=false);
	void processAutoindex(std::string dir, int ind, int arg_num, bool is_loc=false);
	void processIndex(std::string dir, int ind, int arg_num, bool is_loc=false);
    void processCGI(std::string dir, int ind, int arg_num, bool is_loc=false);


    std::vector<Config> getServersArray();
	int getNumServs();

	bool checkIP(std::string ip);
};


#endif //WEBSERV_CONFIGPARSER_HPP

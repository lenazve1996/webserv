//#include "Listen.hpp"
//
//int main()
//{
//    listen_clients();
//}

#include <iostream>

// #include "ConfigParser.hpp"
#include "Listen.hpp"

void printConfigParams(ConfigParser parser)
{
    std::cout << std::endl << ">>>>>>> CONFIG DATA <<<<<<< " << std::endl << std::endl;
    std::vector<Config> servers = parser.getServersArray();

    // std::vector<std::string> serverNames = ;
    for (int ind = 0; ind < parser.getNumServs(); ind++)
    {
        std::cout << ">>>> SERVER № " << ind + 1 << "<<<<" << std::endl << std::endl;
        std::cout << servers[ind]._host << " " << servers[ind]._port << std::endl;
        for (std::vector<std::string>::iterator it = servers[ind]._serverName.begin(); it != servers[ind]._serverName.end(); it++)
        {
            std::cout << *it << std::endl;
        }
        std::cout << servers[ind]._bodySize << std::endl;
        std::cout << servers[ind]._root << std::endl;
        for (std::vector<std::string>::iterator it = servers[ind]._allowedMethods.begin(); it != servers[ind]._allowedMethods.end(); it++)
        {
            std::cout << *it << std::endl;
        }
        for (std::vector<std::string>::iterator it = servers[ind]._index.begin(); it != servers[ind]._index.end(); it++)
        {
            std::cout << *it << std::endl;
        }
        std::cout << servers[ind]._autoindex << std::endl;
        std::cout << servers[ind]._defaultServer << std::endl;
    }
}

int main(int argc, char **argv) {

//    ConfigParser    parser;

//    if (argv[1].substr((argv[1].size() - strlen(".conf")), strlen(".conf")) != ".conf")
//        throw std::invalid_argument("Wrong file extension");
    if (argc != 2){
        std::cout << "Wrong number of arguments" << std::endl;
        return 1;
    } else {
        ConfigParser    parser;
        parser._processConfig(argv[1]);
        printConfigParams(parser);
        listen_clients(parser);
    }
    return 0;
}

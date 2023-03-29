// Here is print functions for all the classes/structures of this projects for debugging purposes

#include "Print.hpp"

void printConfigParams(ConfigParser parser)
{
    std::cout << std::endl << ">>>>>>> CONFIG DATA <<<<<<< " << std::endl << std::endl;
    std::vector<Config> servers = parser.getServersArray();

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
        std::cout << servers[ind]._defaultServer << std::endl << std::endl;
    }
}
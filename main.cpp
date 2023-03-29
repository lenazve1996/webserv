//#include "Listen.hpp"
//
//int main()
//{
//    listen_clients();
//}

#include <iostream>

#include "ConfigParser.hpp"

int main(int argc, char **argv) {

//    ConfigParser    parser;

//    if (argv[1].substr((argv[1].size() - strlen(".conf")), strlen(".conf")) != ".conf")
//        throw std::invalid_argument("Wrong file extension");
    if (argc != 2){
        std::cout << "wrong  number of arguments" << std::endl;
        return 1;
    } else {
        ConfigParser    parser;
        parser._processConfig(argv[1]);
    }
    return 0;
}

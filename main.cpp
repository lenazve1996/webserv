#include "Listen.hpp"
#include "Print.hpp"

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

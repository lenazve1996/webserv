#ifndef INFO_H
# define INFO_H

#include <list>

class Config {

public:

    Config( std::list<int> ports );
    ~Config();
    std::list<int>	getPorts( void );

private:

    std::list<int> _ports;

};

#endif
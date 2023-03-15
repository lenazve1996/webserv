#ifndef INFO_H
# define INFO_H

class Config {

public:

    Config( int port );
    ~Config();
    int		getPort( void );

private:

    int		_port;

};

#endif
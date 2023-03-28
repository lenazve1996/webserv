#ifndef SOCKET_H
# define SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <stdio.h>
#include <list>
#include <string>
#include <iostream>

static int *FD; 
static int *TMP_SOCKET;

class Socket {

public:

    Socket();
    ~Socket();
    void				bindSock( int port );
    void				listenSock( void );
    void				readRequest ( void );
    int					getFd( void );
    struct sockaddr_in  *getAddress( void );

private:

    int                 _fd;
    struct sockaddr_in _address;

};

#endif
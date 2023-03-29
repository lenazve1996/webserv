#include "Socket.hpp"

Socket::Socket()
{
	const int enable = 1;

    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
    }
	FD = &_fd;
    std::cout << "Socket with fd " <<  _fd  << " created" << std::endl;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		perror("setsockopt for SO_REUSEADDR failed");
        close(_fd);
		exit(EXIT_FAILURE);
	}
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
	{
		perror("setsockopt for SO_REUSEPORT failed");
        close(_fd);
		exit(EXIT_FAILURE);
	}

}

Socket::~Socket()
{
	close(_fd);
}

void Socket::bindSock( int port )
{
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    _address.sin_port = htons(port);

    if (bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    { 
        perror("bind failed"); 
        close(_fd);
        exit(EXIT_FAILURE);
    }
}

void Socket::listenSock( void )
{
    if (listen(_fd, 10) > 0)
    {
        perror("listen func failed");
        close(_fd);
        exit(EXIT_FAILURE);
    }
}

int	Socket::getFd( void )
{
	return _fd;
}

struct sockaddr_in  *Socket::getAddress( void )
{
    return &_address;
}

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
    // if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1){
    //     perror("fcntl for O_NONBLOCK failed");
    //     close(_fd);
	// 	exit(EXIT_FAILURE);
    // }
}

Socket::~Socket()
{
	close(_fd);
}

static in_addr_t ft_inet_addr(const char *str) {
    size_t      i;
    in_addr_t   res;

    res = atoi(str) << 24;
    i = 0;
    while (str[i] && str[i] != '.')
        i++;
    if (str[i] != '.' || !str[i + 1])
        return(INADDR_NONE);
    i++;
    res |= atoi(str + i) << 16;
    while (str[i] && str[i] != '.')
        i++;
    if (str[i] != '.' || !str[i + 1])
        return(INADDR_NONE);
    i++;
    res |= atoi(str + i) << 8;
    while (str[i] && str[i] != '.')
        i++;
    if (str[i] != '.' || !str[i + 1])
        return(INADDR_NONE);
    i++;
    res |= atoi(str + i);
    return (htonl(res));
}

void Socket::bindSock( int port )
{
    _address.sin_family = AF_INET; /* internetwork: UDP, TCP, etc. */
    // _address.sin_addr.s_addr = ft_inet_addr("195.133.239.85"); // Можно установить ip адрес прямо из строки
    // _address.sin_addr.s_addr = ft_inet_addr("192.168.22.117"); // Можно установить ip адрес прямо из строки
    _address.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY -- любой адрес, то же самое что и 0.0.0.0 (будут обратбатываться, и 127.0.0.1, и адрес локальной сети)
    _address.sin_port = htons(port); // port
    printf("binding address is %s\n",inet_ntoa(_address.sin_addr)); // показать адрес

    if (bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    {
        perror("bind failed"); 
        close(_fd);
        exit(EXIT_FAILURE);
    }
}

void Socket::listenSock( void )
{
    if (listen(_fd, 40) > 0)
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

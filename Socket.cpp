#include "Socket.hpp"

Socket::Socket()
{
	const int enable = 1;

    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
    }
	FD = &_fd;
    std::cout << "Number of the socket fd: " <<  _fd << std::endl;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		perror("setsockopt for SO_REUSEADDR failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
	{
		perror("setsockopt for SO_REUSEPORT failed");
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

void Socket::readRequest( void )
{
    int	tmpSocket;
	int readResult;
	int addrlen = sizeof(_address);
    while (true)
    {
       	std::cout  << std::endl << ">>>> WEBSERVER IS WAITING FOR NEW CONNECTION <<<<" << std::endl << std::endl;
        if ((tmpSocket = accept(_fd, (struct sockaddr *)&_address, (socklen_t*)&addrlen))<0)
        {
            perror("Accept func failed");     
            exit(EXIT_FAILURE);
        }
		TMP_SOCKET = &tmpSocket;
		std::cout << "Number of the tmpSocket fd: " << tmpSocket << std::endl;

        char buffer[3000];
        readResult = read(tmpSocket , buffer, 3000);
        if (readResult == -1)
        {
            perror("Read func failed");     
            exit(EXIT_FAILURE);
        }
        std::cout << ">>>> REQUEST: <<<<" << std::endl << std::endl  << buffer;

		// std::string answer;
        // answer = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 21\n\nHello from webserver!";
		// std::cout << ">>>> ANSWER: <<<<" << std::endl << answer << std::endl;
		// std::cout << answer.length() << std::endl;
		char *answer;
		answer = (char *)malloc(sizeof(char) * 3000);
		answer = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 21\n\nHello from webserver!";
		std::cout << ">>>> ANSWER: <<<<" << std::endl << answer << std::endl;
		write(tmpSocket, answer, strlen(answer));
        close(tmpSocket);
    }
}

int	Socket::getFd( void )
{
	return _fd;
}

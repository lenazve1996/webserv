#include "Listen.hpp"

void	sig_int_handler( int i )
{
    (void)i;
	write(1, "\n", 1);
    close(*FD);
    close(*TMP_SOCKET);
}

std::list<Socket*> createSocketsList(ConfigParser _parser)
{
    std::list<Socket*> sockets;

    std::cout  << std::endl << ">>>> CREATING SOCKETS <<<<" << std::endl << std::endl;
    int numServs = _parser.getNumServs();
    for (int i = 0; i != numServs; i++)
    {
        sockets.push_back( new Socket());
    }

    std::list<Socket *>::iterator sockets_it = sockets.begin();
    for (int i = 0; i != numServs; i++)
    {

        (*sockets_it)->bindSock(_parser.getServersArray()[i]._port);
        std::cout << "Socket with fd " << (*sockets_it)->getFd() << " binded to port: " << _parser.getServersArray()[i]._port << std::endl;
        sockets_it++;
    }
    return (sockets);
}

void listenSockets(std::list<Socket *> sockets)
{
    std::list<Socket *>::iterator sockets_it = sockets.begin();
    for (sockets_it = sockets.begin(); sockets_it != sockets.end(); sockets_it++)
    {
        (*sockets_it)->listenSock();
    }
}

void makeFdSet(std::list<Socket *> sockets, fd_set *readFds, int *maxNum)
{
    FD_ZERO(readFds);
    std::list<Socket *>::iterator sockets_it = sockets.begin();
    *maxNum = 0;
    for (sockets_it = sockets.begin(); sockets_it != sockets.end(); sockets_it++)
    {
        FD_SET((*sockets_it)->getFd(), readFds);
        if ((*sockets_it)->getFd() > *maxNum)
            *maxNum = (*sockets_it)->getFd();
    }
}

int acceptConnection(std::list<Socket *>::iterator socket, fd_set *readFds)
{
    int readSocket = 0;

    if (FD_ISSET((*socket)->getFd(), readFds))
    {
        if ((readSocket = accept((*socket)->getFd(), (struct sockaddr *)(*socket)->getAddress(), (socklen_t*)(*socket)->getAddress()))<0)
        {
            perror("Accept func failed");
            exit(EXIT_FAILURE);
        }
    }
    return readSocket;
}

void readRequest(int readSocket)
{
    char buffer[4000];

    int readResult = read(readSocket, buffer, 3999);
    if (readResult == -1)
    {
        perror("Read func failed"); 
        exit(EXIT_FAILURE);
    }
    std::cout << ">>>> REQUEST: <<<<" << std::endl << std::endl  << buffer;
}

void successResponse(int readSocket)
{
    std::string response;
    response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 21\n\nHello from webserver!";
    std::cout << ">>>> RESPONSE: <<<<" << std::endl << response << std::endl;
    write(readSocket, response.c_str(), response.length());
}

void errorResponse(int readSocket)
{
    std::string response;
    std::ifstream file404("./www/404/error_404.html");
    std::stringstream buffer;
    buffer << file404.rdbuf();
    int size = buffer.str().length();
    std::cout << "PAGE_SIZE: " << size << std::endl;
    response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 52381\n\n";
    write(readSocket, response.c_str(), response.length());
    write(readSocket, buffer.str().c_str(), buffer.str().length());
    std::cout << ">>>> RESPONSE: <<<<" << std::endl << response << std::endl;
}

void response(int readSocket)
{
    std::string response;
    int statusCode = 200; //hardcode

    if (statusCode == 200)
        successResponse(readSocket);
    else if (statusCode == 404)
        errorResponse(readSocket);
    close(readSocket);
}

void selectConnections(std::list<Socket *> sockets, fd_set *readFds, int *maxNum)
{
    fd_set readFdsTmp;
    fd_set writeFds;
    fd_set writeFdsTmp;
    int readSocket;
    struct timeval tv;
    tv.tv_sec = 20; 
    tv.tv_usec = 0;
    std::list<Socket *>::iterator sockets_it;


    memcpy(&readFdsTmp, readFds, sizeof(*readFds));
    FD_ZERO(&writeFds);
    FD_ZERO(&writeFdsTmp);
    // memcpy(&writeFds, readFds, sizeof(*readFds));
    while (true)
    {
        std::cout  << std::endl << ">>>> WEBSERVER IS WAITING FOR NEW CONNECTION <<<<" << std::endl << std::endl;
        int ret = select(*maxNum + 1, readFds, &writeFds, NULL, &tv);
        if (ret > 0)
        {
            for (sockets_it = sockets.begin(); sockets_it != sockets.end(); sockets_it++)
            {
                std::cout << "FD_ISSET status: for " << (*sockets_it)->getFd() << " is " << FD_ISSET((*sockets_it)->getFd(), readFds) << std::endl;
                readSocket = acceptConnection(sockets_it, readFds);
                if (readSocket)
                {
                    readRequest(readSocket);
                    FD_SET(readSocket, &writeFds);
                    response(readSocket);
                }
            }
        }
        else if (ret < 0)
        {
            perror("Select func failed");
            exit(EXIT_FAILURE);
        }
        else if (ret == 0)
            std::cout << "TIMEOUT" << std::endl;
        FD_ZERO(readFds);
        FD_ZERO(&writeFds);
        memcpy(readFds, &readFdsTmp, sizeof(*readFds));
        memcpy(&writeFds, &readFdsTmp, sizeof(*readFds));
    }
}

void handleConnections(std::list<Socket *> sockets)
{
    fd_set *readFds = (fd_set *)malloc(sizeof(fd_set));
    int maxNum[1];
    makeFdSet(sockets, readFds, maxNum);
    selectConnections(sockets, readFds, maxNum);
    free(readFds);
}

void freeSocketsList(std::list<Socket *> sockets)
{
    std::list<Socket *>::iterator tmp_it;
    std::list<Socket *>::iterator sockets_it = sockets.begin();

    while (sockets_it != sockets.end())
    {
        tmp_it = sockets_it;
        sockets_it++;
        delete *tmp_it;
        sockets.push_back(new Socket());
    }
    sockets.clear();
}

void listen_clients(ConfigParser parser)
{
    std::vector<Config> servers = parser.getServersArray();

    std::list<Socket*> sockets = createSocketsList(parser);
    listenSockets(sockets);
    handleConnections(sockets);
    freeSocketsList(sockets);
}
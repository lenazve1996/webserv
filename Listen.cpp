#include "Listen.hpp"

void	sig_int_handler( int i )
{
    (void)i;
	write(1, "\n", 1);
    close(*FD);
    close(*TMP_SOCKET);
}

void	redefine_signals()
{
	struct sigaction	sig_int;
	sig_int.sa_flags = 0;
	sig_int.sa_handler = sig_int_handler;

}

std::list<int> createPortsList()
{
    std::list<int> examplePorts;
    examplePorts.push_back(8080);
    examplePorts.push_back(8081);
    examplePorts.push_back(8082);
    examplePorts.push_back(8083);
    Config fakeConfig = Config(examplePorts);

    return (fakeConfig.getPorts());
}

std::list<Socket*> createSocketsList(void)
{
    std::list<Socket*> sockets;
    std::list<int>::iterator ports_it;

    std::list<int> configPorts = createPortsList(); 
    for (ports_it = configPorts.begin(); ports_it != configPorts.end(); ports_it++)
    {
        sockets.push_back( new Socket());
    }

    std::list<Socket *>::iterator sockets_it = sockets.begin();
    for (ports_it = configPorts.begin(); ports_it != configPorts.end(); ports_it++)
    {

        (*sockets_it)->bindSock(*ports_it);
        std::cout << "Binded port: " << *ports_it << std::endl;
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
    std::cout << "2 " << readFds << std::endl;
    std::list<Socket *>::iterator sockets_it = sockets.begin();
    *maxNum = 0;
    for (sockets_it = sockets.begin(); sockets_it != sockets.end(); sockets_it++)
    {
        std::cout << (*sockets_it)->getFd() << std::endl;
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

void answer(int readSocket)
{
    std::string answer;

    answer = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 21\n\nHello from webserver!";
    std::cout << ">>>> ANSWER: <<<<" << std::endl << answer << std::endl;
    write(readSocket, answer.c_str(), answer.length());
    close(readSocket);
}

void selectConnections(std::list<Socket *> sockets, fd_set *readFds, int * maxNum)
{
    fd_set readFdsTmp;
    int readSocket;
    struct timeval tv;
    tv.tv_sec = 10; 
    tv.tv_usec = 0;
    std::list<Socket *>::iterator sockets_it;

    memcpy(&readFdsTmp, readFds, sizeof(*readFds));
    while (true)
    {
        std::cout  << std::endl << ">>>> WEBSERVER IS WAITING FOR NEW CONNECTION <<<<" << std::endl << std::endl;
        int ret = select(*maxNum + 1, readFds, NULL, NULL, &tv);
        if (ret)
        {
            for (sockets_it = sockets.begin(); sockets_it != sockets.end(); sockets_it++)
            {
                std::cout << "FD_ISSET status: for " << (*sockets_it)->getFd() << " is " << FD_ISSET((*sockets_it)->getFd(), readFds) << std::endl;
                readSocket = acceptConnection(sockets_it, readFds);
                if (readSocket)
                {
                    readRequest(readSocket);
                    answer(readSocket);
                }
            }
        }
        else
            std::cout << "TIMEOUT" << std::endl;
        FD_ZERO(readFds);
        memcpy(readFds, &readFdsTmp, sizeof(*readFds));
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

void listen_clients(void)
{
    std::list<Socket*> sockets = createSocketsList();
    redefine_signals();
    listenSockets(sockets);
    handleConnections(sockets);
    freeSocketsList(sockets);
}
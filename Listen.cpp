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

void readRequests(std::list<Socket *> sockets)
{
    std::list<Socket *>::iterator sockets_it = sockets.begin();
    for (sockets_it = sockets.begin(); sockets_it != sockets.end(); sockets_it++)
    {
        (*sockets_it)->readRequest();
    }
}

// void readRequests(std::list<Socket *> sockets)
// {
//     struct pollfd fds[2];

    
// }

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
    readRequests(sockets);
    freeSocketsList(sockets);
}
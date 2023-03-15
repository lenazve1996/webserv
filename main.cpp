#include "Socket.hpp"
#include "Config.hpp"
#include <sys/types.h>

void	sig_int_handler( int i )
{
    (void)i;
	write(1, "\n", 1);
    close(*FD);
    close(*NEW_SOCKET);
}

void	redefine_signals()
{
	struct sigaction	sig_int;
	sig_int.sa_flags = 0;
	sig_int.sa_handler = sig_int_handler;

}

int main()
{

    Config fakeConfig = Config(8080);
    Socket first = Socket();

    first.bindSock(fakeConfig.getPort());
    redefine_signals();
    first.listenSock();
    first.readRequest();
}
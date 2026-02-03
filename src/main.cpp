#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

int main()
{
	Server server;
	std::cout << "---- SERVER INIT ----" << std::endl;
	try
    {
		signal(SIGINT, Server::handleSignal);
		signal(SIGQUIT, Server::handleSignal);
		server.serverInit();
	}
	catch(const std::exception& e)
    {
		server.closeFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}

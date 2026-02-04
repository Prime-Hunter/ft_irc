#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

static bool isPortValid(std::string port)
{
	return (port.find_first_not_of("0123456789") == std::string::npos && \
    std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535);
}

static int isPasswordValid(std::string password)
{
    int i = 0;
    while (password[i] != '\0')
    {
        if (32 > password[i] || password[i] > 126)
        {
            std::cerr << "Password must be printable" << std::endl;
            return (0);
        }
        i++;
    }
    if (i == 0)
    {
        std::cerr << "Password cannot be an empty string" << std::endl;
        return (0);
    }

    return (1);
}

static int check_args(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return (0);
    }
    if (!isPortValid(argv[1]))
    {
        std::cout << "Invalid port (1024 to 65535)" << std::endl;
        return (0);
    }
    if (!isPasswordValid(argv[2]))
    {
        return (0);
    }

    return (1);
}

int main(int argc, char **argv)
{
    if (!check_args(argc, argv))
    {
        return (1);
    }

	Server server;
	std::cout << "---- SERVER INIT ----" << std::endl;
	try
    {
		signal(SIGINT, Server::handleSignal);
		signal(SIGQUIT, Server::handleSignal);
		server.serverInit(std::atoi(argv[1]), argv[2]);
	}
	catch(const std::exception& e)
    {
		server.closeFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}

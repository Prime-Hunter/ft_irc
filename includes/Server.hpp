#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <string.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "Client.hpp"
#include "Command.hpp"

#define BUFFER_SIZE 1024

class Server
{
    private:
        int _port;
        std::string _password;
        int _socketFd;
        static bool _signal;
        std::vector<Client> _clientList;
        std::vector<struct pollfd> _fds;
    
    public:
        Server();
        ~Server();
        Server(Server const &copy);
	    Server &operator=(Server const &src);

        void serverInit(int port, std::string password);
        void createSocket();

        void acceptClient();
        void newClientData(int clientFd);

        static void handleSignal(int signum);

        void closeFds();
        void clearClient(int clientFd);
        Client *getClient(int fd);
};

#endif
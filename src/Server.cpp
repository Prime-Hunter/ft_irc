#include "../includes/Server.hpp"

bool Server::_signal = false;

Server::Server(void) 
{
    this->_socketFd = -1;
    std::cout << "Default server constructor called" << std::endl;
}

Server::~Server()
{
    std::cout << "Server destructor called" << std::endl;
}

Server::Server(Server const &copy)
{
    *this = copy;
    std::cout << "Server copy constructor called" << std::endl;
}

Server &Server::operator=(Server const &src){
	if (this == &src)
    {
		return (*this);
    }
	this->_port = src._port;
	this->_socketFd = src._socketFd;
	this->_clientList = src._clientList;
	this->_fds = src._fds;
	return (*this);
}

void Server::clearClient(int clientFd)
{
	for (size_t i = 0; i < this->_fds.size(); i++)
    {
		if (this->_fds[i].fd == clientFd)
		{
            this->_fds.erase(this->_fds.begin() + i); 
            break;
        }
	}
	for (size_t i = 0; i < this->_clientList.size(); i++)
    {
		if (this->_clientList[i].getFd() == clientFd)
		{
            this->_clientList.erase(this->_clientList.begin() + i); 
            break;
        }
	}

}

void Server::closeFds(void)
{
    for (size_t i =0 ; i < this->_clientList.size(); i++)
    {
        std::cout << this->_clientList[i].getFd() << " disconnected" << std::endl;
        close(this->_clientList[i].getFd());
    }
    if (this->_socketFd != -1)
    {
        std::cout << "Server " << this->_socketFd << " disconnected" << std::endl;
        close(this->_socketFd);
    }
}

void Server::handleSignal(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::_signal = true;
}

void Server::createSocket(void)
{
	struct sockaddr_in address;
	struct pollfd newPoll;
	address.sin_family = AF_INET;
	address.sin_port = htons(this->_port);
	address.sin_addr.s_addr = INADDR_ANY;

	this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFd == -1)
	{
		throw(std::runtime_error("faild to create socket"));
	}

    int val = 1;
    if (setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
    {
        throw(std::runtime_error("Could not set SO_REUSEADDR option on server socket"));
    }
    if (fcntl(this->_socketFd, F_SETFL, O_NONBLOCK) == -1)
    {
        throw(std::runtime_error("Could not set O_NONBLOCK option on server socket"));
    }
    if (bind(this->_socketFd, (struct sockaddr *) &address, sizeof(address)) == -1)
    {
        throw(std::runtime_error("Could not bind socket address to server socket"));
    }
    if (listen(this->_socketFd, SOMAXCONN) == -1)
    {
        throw(std::runtime_error("Could not set server socket to passive"));
    }

    newPoll.fd = this->_socketFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    this->_fds.push_back(newPoll);
}

void Server::serverInit(void)
{
    std::cout << "Initializing server ..." << std::endl;
    this->_port = SERV_PORT;
    this->createSocket();
    std::cout << "Server initialized successfully !" << std::endl;
    std::cout << "Waiting for client connection ..." << std::endl;
}


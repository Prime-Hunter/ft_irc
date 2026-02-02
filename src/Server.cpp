#include "../includes/Server.hpp"

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
	this->password = src.password;
	this->_clientList = src._clientList;
	this->_fds = src._fds;
	return (*this);
}
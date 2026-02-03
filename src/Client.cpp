#include "../includes/Client.hpp"

Client::Client(void)
{
	this->_nickname = "";
	this->_username = "";
	this->_fd = -1;
    std::cout << "Default client constructor called" << std::endl;
}

Client::Client(int fd, std::string username, std::string nickname):_fd(fd), _username(username), _nickname(nickname)
{
    std::cout << "Client construcotr called for " << username << ", fd: " << fd << std::endl;
}

Client::~Client(void)
{
    std::cout << "Client destructor called" << std::endl;
}

Client::Client(Client const &copy)
{
    *this = copy;
    std::cout << "Client copy constructor called" << std::endl;
}

Client &Client::operator=(Client const &src)
{
	if (this == &src) 
    {
        return (*this);
    }
	this->_nickname = src._nickname;
	this->_username = src._username;
	this->_fd = src._fd;
	return (*this);
}

int Client::getFd(void)
{
    return (this->_fd);
}

std::string Client::getNickname(void)
{
    return (this->_nickname);
}

std::string Client::getUsername(void)
{
    return (this->_username);
}

void Client::setFd(int fd)
{
    this->_fd = fd;
}

void Client::setIpAddress(std::string ipAddress)
{
    this->_ipAddress = ipAddress;
}

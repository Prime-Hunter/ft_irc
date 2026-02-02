#include "../includes/Client.hpp"

Client::Client(void)
{
	this->_nickname = "";
	this->_username = "";
	this->_fd = -1;
    std::cout << "Default client constructor called" << std::endl;
}

Client::Client(std::string nickname, std::string username, int fd) :_fd(fd), _nickname(nickname), _username(username)
{
    std::cout << "Client construcotr called for " << username << ", fd: " << fd << std::endl;
}

Client::~Client(void)
{
    std::cout << "Client destructor called for " << this->getUsername << std::endl;
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


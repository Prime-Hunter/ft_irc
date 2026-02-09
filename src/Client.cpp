#include "../includes/Client.hpp"

Client::Client(void)
{
	this->_nickname = "";
	this->_username = "";
	this->_fd = -1;
    this->_login = 0;
}

Client::Client(int fd, std::string username, std::string nickname):_fd(fd), _username(username), _nickname(nickname)
{
    this->_login = 0;
}

Client::~Client(void)
{}

Client::Client(Client const &copy)
{
    *this = copy;
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
    this->_login = src._login;
	return (*this);
}

int Client::getFd(void)
{
    return (this->_fd);
}

int Client::getLogin(void)
{
    return (this->_login);
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

void Client::setUsername(std::string username)
{
    this->_username = username;
}

void Client::setNickname(std::string nickname)
{
    this->_nickname = nickname;
}

void Client::logIn()
{
    this->_login = 1;
}
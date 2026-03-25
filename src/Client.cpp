#include "../includes/Client.hpp"
#include <sys/socket.h>

Client::Client(void)
{
	this->_nickname = "";
	this->_username = "";
	this->_fd = -1;
    this->_login = 0;
    this->_write = 0;
    this->_currentChannel = NULL;
}

Client::Client(int fd, std::string username, std::string nickname):_fd(fd), _username(username), _nickname(nickname)
{
    this->_login = 0;
    this->_write = 0;
    this->_currentChannel = NULL;
}

Client::~Client(void) {}

Client::Client(Client const &copy) {*this = copy;}

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
    this->_write = src._write;
	return (*this);
}

int Client::getFd(void) {return (this->_fd);}

int Client::getLogin(void) {return (this->_login);}

std::string Client::getNickname(void){return (this->_nickname);}

std::string Client::getUsername(void) {return (this->_username);}

int Client::getWrite(void) {return (this->_write);}

Channel Client::getChannel(void) 
{
    if (this->_currentChannel)
        return (*this->_currentChannel);
    else
        return (Channel());
}

void Client::setFd(int fd) {this->_fd = fd;}

void Client::setIpAddress(std::string ipAddress) {this->_ipAddress = ipAddress;}

void Client::setUsername(std::string username) {this->_username = username;}

void Client::setNickname(std::string nickname) {this->_nickname = nickname;}

void Client::setChannel(Channel *newChannel) {this->_currentChannel = newChannel;}

void Client::logIn(void)
{
    this->_login = 1;
    send(this->getFd(), "You are now logged in !\n", sizeof("You are now logged in !\n"), 0);
}

void Client::checkAuth(void)
{
    if (this->getLogin() && !this->getUsername().empty() && !this->getNickname().empty())
    {
        this->_write = 1;
    }
}

bool Client::hasChannel(void)
{
    return (this->_currentChannel != NULL);
}
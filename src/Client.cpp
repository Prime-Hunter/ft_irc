#include "../includes/Client.hpp"
#include "../includes/Replies.hpp"
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

std::string Client::getHostname(void) {return (this->_hostname);}

std::string Client::getServername(void) {return (this->_servername);}

std::string Client::getRealname(void) {return (this->_realname);}

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

void Client::setHostname(std::string hostname) {this->_hostname = hostname;}

void Client::setServername(std::string servername) {this->_servername = servername;}

void Client::setRealname(std::string realname) {this->_realname = realname;}

void Client::setChannel(Channel *newChannel) {this->_currentChannel = newChannel;}

void Client::appendTempBuffer(const std::string &data){ this->_tempBuffer += data; }

std::string Client::getTempBuffer(void){ return this->_tempBuffer; }

void Client::clearTempBuffer(void){ this->_tempBuffer.clear(); }

void Client::logIn(void)
{
    this->_login = 1;
}

void Client::checkAuth(void)
{
    if (this->getLogin() && !this->getUsername().empty() && !this->getNickname().empty())
    {
        this->_write = 1;
        send(this->getFd(), IRC::Reply::welcome(this->getNickname(), this->getUsername(), this->getHostname()).c_str(), IRC::Reply::welcome(this->getNickname(), this->getUsername(), this->getHostname()).length(), 0);
        send(this->getFd(), IRC::Reply::yourhost(this->getNickname()).c_str(), IRC::Reply::yourhost(this->getNickname()).length(), 0);
        send(this->getFd(), IRC::Reply::created(this->getNickname()).c_str(), IRC::Reply::created(this->getNickname()).length(), 0);
        send(this->getFd(), IRC::Reply::myinfo(this->getNickname()).c_str(), IRC::Reply::myinfo(this->getNickname()).length(), 0);
    }
}

bool Client::hasChannel(void)
{
    return (this->_currentChannel != NULL);
}
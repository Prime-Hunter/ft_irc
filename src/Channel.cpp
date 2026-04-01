#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include <algorithm>
#include <iostream>
#include <set>

Channel::Channel(): _name(""), _topic(""), _inviteOnly(0), _secured(0), _connectedCount(0) {}

Channel::Channel(const std::string &name): _name(name), _topic(""), _inviteOnly(0) , _secured(0), _connectedCount(0){}

Channel::~Channel() {}

const std::string Channel::getName() const {return (this->_name);}

const std::string Channel::getTopic() const {return (this->_topic);}

const std::string Channel::getKey() const {return (this->_key);}

int Channel::getClientCount() const {return (this->_connectedCount);}

int Channel::isInviteOnly() const {return (this->_inviteOnly);}

int Channel::isRestricted() const {return (this->_secured);}

void Channel::setTopic(const std::string topic) {this->_topic = topic;}

void Channel::addMember(Client *client) 
{
    this->_connectedClients.push_back(client);
    this->_connectedCount++;
}

void Channel::addOperator(Client *client) 
{
    this->_ops.push_back(client);
    this->_connectedCount++;
}

void Channel::removeMember(Client *client)
{
    std::vector<Client *>::iterator i;
    i = std::find(this->_connectedClients.begin(), this->_connectedClients.end(), client);
    if (i != this->_connectedClients.end())
    {
        this->_connectedClients.erase(i);
        this->_connectedCount--;
    }
    std::vector<Client *>::iterator op;
    op = std::find(this->_ops.begin(), this->_ops.end(), client);
    if (op != this->_ops.end())
    {
        this->_ops.erase(op);
        this->_connectedCount--;
    }
}

int Channel::isMember(Client *client) const
{
    std::vector<Client *>::const_iterator i;
    i = std::find(_connectedClients.begin(), _connectedClients.end(), client);
    if (i != this->_connectedClients.end())
    {
        return (1);
    }
    i = std::find(_ops.begin(), _ops.end(), client);
    if (i != this->_ops.end())
    {
        return (1);
    }
    return (0);
}

int Channel::isOperator(Client *client) const
{
    std::vector<Client *>::const_iterator i;
    i = std::find(_ops.begin(), _ops.end(), client);
    if (i != _ops.end())
        return (1);
    return (0);
}

void Channel::setInviteOnly(int value) { this->_inviteOnly = value; }

void Channel::setRestricted(int value) { this->_secured = value; }

void Channel::broadcast(std::string mess, Client *author)
{
    (void) author;
    std::set<Client*> list;
    for (std::vector<Client*>::iterator it = _connectedClients.begin(); it != _connectedClients.end(); ++it)
    {
        list.insert(*it);
    }
    for (std::vector<Client*>::iterator it = _ops.begin(); it != _ops.end(); ++it)
    {
        list.insert(*it);
    }
    for (std::set<Client*>::iterator it = list.begin(); it != list.end(); ++it)
    {
        send((*it)->getFd(), mess.c_str(), mess.length(), 0);
    }
}

void Channel::sendMessage(std::string mess, Client *author)
{
    std::set<Client*> list;
    for (std::vector<Client*>::iterator it = _connectedClients.begin(); it != _connectedClients.end(); ++it)
    {
        if ((*it)->getNickname() != author->getNickname())
            list.insert(*it);
    }
    for (std::vector<Client*>::iterator it = _ops.begin(); it != _ops.end(); ++it)
    {
        if ((*it)->getNickname() != author->getNickname())
            list.insert(*it);
    }
    for (std::set<Client*>::iterator it = list.begin(); it != list.end(); ++it)
    {
        send((*it)->getFd(), mess.c_str(), mess.length(), 0);
    }
}

std::string Channel::getUsers(void)
{
    std::string res = "";
    for (std::vector<Client *>::iterator it = _ops.begin(); it != _ops.end(); ++it)
    {
        if (!(*it)->getNickname().empty())
        {
            res += "@" + (*it)->getNickname() + " ";
        }
    }
    for (std::vector<Client *>::iterator it = _connectedClients.begin(); it != _connectedClients.end(); ++it)
    {
        if (!(*it)->getNickname().empty())
        {
            res += (*it)->getNickname() + " ";
        }
    }
    return (res);
}
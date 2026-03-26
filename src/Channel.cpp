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
    return (0);
}

void Channel::sendMessage(std::string mess, Client *author)
{
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
        if ((*it)->getFd() != author->getFd())
        {
            send((*it)->getFd(), "<#", 2, 0);
            send((*it)->getFd(), this->getName().c_str(), this->getName().length(), 0);
            send((*it)->getFd(), " @", 2, 0);
            send((*it)->getFd(), author->getUsername().c_str(), author->getUsername().length(), 0);
            send((*it)->getFd(), ">: ", 3, 0);
            send((*it)->getFd(), mess.c_str(), mess.length(), 0);
            send((*it)->getFd(), "\n", 1, 0);
        }
    }
}
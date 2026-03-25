#include "../includes/Channel.hpp"
#include <algorithm>

Channel::Channel(): _name("unknown"), _topic(""), _inviteOnly(0) , _secured(0) {}

Channel::Channel(const std::string &name): _name(name), _topic(""), _inviteOnly(0) , _secured(0) {}

Channel::~Channel() {}

const std::string Channel::getName() const {return (this->_name);}

const std::string Channel::getTopic() const {return (this->_topic);}

const std::string Channel::getKey() const {return (this->_key);}

int Channel::isInviteOnly() const {return (this->_inviteOnly);}

int Channel::isRestricted() const {return (this->_secured);}

void Channel::setTopic(const std::string topic) {this->_topic = topic;}

void Channel::addMember(Client *client) {this->_connectedClients.push_back(client);}

void Channel::addOperator(Client *client) {this->_ops.push_back(client);}

void Channel::removeMember(Client *client)
{
    std::vector<Client *>::iterator i;
    i = std::find(this->_connectedClients.begin(), this->_connectedClients.end(), client);
    if (i != this->_connectedClients.end())
    {
        this->_connectedClients.erase(i);
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
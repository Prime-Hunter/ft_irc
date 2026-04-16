#include "../includes/Command.hpp"

void Command::kick()
{
    if (this->_args.size() < 3 || this->_args[1].empty() || this->_args[2].empty())
    {
        std::string reply = Reply::needmoreparams(this->_target->getNickname(), this->_name);
        send(this->_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }

    Channel *channel = this->getChannel(this->_args[1]);
    Client *client = this->getNClient(this->_args[2]);
    
    if (channel == NULL)
    {
        std::string reply = Reply::nosuchchannel(this->_target->getNickname(), this->_args[1]);
        send(this->_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (!channel->isMember(this->_target))
    {
        std::string reply = Reply::notonchannel(this->_target->getNickname(), this->_args[1]);
        send(this->_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (!channel->isOperator(this->_target))
    {
        std::string reply = Reply::chanoprivsneeded(this->_target->getNickname(), channel->getName());
        send(this->_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (client == NULL)
    {
        std::string reply = Reply::nosuchnick(this->_target->getNickname(), this->_args[2]);
        send(this->_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (!channel->isMember(client))
    {
        std::string reply = Reply::notonchannel(this->_target->getNickname(), this->_args[1]);
        send(this->_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    channel->broadcast(Reply::kick(this->_target->getPrefix(), channel->getName(), client->getNickname(), ""), this->_target);
    channel->removeMember(client);
    client->setChannel(NULL);
}

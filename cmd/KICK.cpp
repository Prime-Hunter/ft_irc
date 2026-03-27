#include "../includes/Command.hpp"

void Command::kick()
{
    if (this->_args.size() < 3)
    {
        send(this->_target->getFd(), Reply::needmoreparams(this->_target->getNickname(), this->_name).c_str(), Reply::needmoreparams(this->_target->getNickname(), this->_name).length(), 0);
        return;
    }
    Channel *channel = this->getChannel(this->_args[1]);
    Client *client = this->getNClient(this->_args[2]);
    if (this->_args[1].empty())
    {
        send(this->_target->getFd(), Reply::nosuchchannel(this->_target->getNickname(), "").c_str(), Reply::nosuchchannel(this->_target->getNickname(), "").length(), 0);
        return;
    }
    if (channel == NULL)
    {
        send(this->_target->getFd(), Reply::nosuchchannel(this->_target->getNickname(), this->_args[1]).c_str(), Reply::nosuchchannel(this->_target->getNickname(), this->_args[1]).length(), 0);
        return;
    }
    if (this->_args[2].empty())
    {
        send(this->_target->getFd(), Reply::nosuchnick(this->_target->getNickname(), "").c_str(), Reply::nosuchnick(this->_target->getNickname(), "").length(), 0);
        return;
    }
    if (client == NULL)
    {
        send(this->_target->getFd(), Reply::nosuchnick(this->_target->getNickname(), this->_args[2]).c_str(), Reply::nosuchnick(this->_target->getNickname(), this->_args[2]).length(), 0);
        return;
    }
    send(client->getFd(), Reply::kick(this->_target->getNickname(), channel->getName(), client->getNickname(), "").c_str(), Reply::kick(this->_target->getNickname(), channel->getName(), client->getNickname(), "").length(), 0);
    channel->removeMember(client);
    client->setChannel(NULL);
}

#include "../includes/Command.hpp"

void Command::kick()
{
    if (this->_args.size() < 3 || this->_args[1].empty() || this->_args[2].empty())
    {
        this->_target->getChannel().broadcast(Reply::needmoreparams(this->_target->getNickname(), this->_name), this->_target);
        return;
    }

    Channel *channel = this->getChannel(this->_args[1]);
    Client *client = this->getNClient(this->_args[2]);
    
    if (channel == NULL)
    {
        channel->broadcast(Reply::nosuchnick(this->_target->getNickname(), this->_args[1]), this->_target);
        return;
    }
    if (client == NULL)
    {
        channel->broadcast(Reply::nosuchnick(this->_target->getNickname(), this->_args[2]), this->_target);
        return;
    }
    channel->removeMember(client);
    client->setChannel(NULL);
    channel->broadcast(Reply::kick(this->_target->getNickname(), channel->getName(), client->getNickname(), ""), this->_target);
}

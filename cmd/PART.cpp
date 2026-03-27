#include "../includes/Command.hpp"

void Command::part()
{
    std::string nick = this->_target->getNickname().empty() ? "*" : this->_target->getNickname();

    if (this->_args.size() != 2)
    {
        std::string message = Reply::needmoreparams(nick, "PART");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (this->_args[1].empty())
    {
        std::string message = Reply::needmoreparams(nick, "PART");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    std::string channelName = this->_args[1];
    std::vector<Channel> *channels = _serv->getChannels();
    std::vector<Channel>::iterator it = channels->begin();
    for (; it != channels->end(); ++it)
    {
        if (it->getName() == channelName)
            break;
    }
    if (it == channels->end())
    {
        std::string message = Reply::nosuchchannel(nick, channelName);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    it->removeMember(_target);
    _target->setChannel(NULL);
    if (it->getClientCount() <= 0)
    {
        channels->erase(it);
    }
}
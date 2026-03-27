#include "../includes/Command.hpp"
#include <algorithm>

static bool compareChannels(const Channel &c, const std::string &name)
{
    return c.getName() == name;
}

void Command::join()
{
    if (this->_args.size() != 2)
    {
        send(this->_target->getFd(), Reply::needmoreparams(this->_target->getNickname(), this->_name).c_str(), Reply::nosuchchannel(this->_target->getNickname(), this->_name).length(), 0);
        return;
    }
    if (this->_args[1].empty())
    {
        send(this->_target->getFd(), Reply::nosuchchannel(this->_target->getNickname(), "").c_str(), Reply::nosuchchannel(this->_target->getNickname(), "").length(), 0);
        return;
    }
    std::string channelName = this->_args[1];
    std::vector<Channel> *channels = _serv->getChannels();
    std::vector<Channel>::iterator it = channels->begin();
    for (; it != channels->end(); ++it)
    {
        if (compareChannels(*it, channelName))
            break;
    }
    if (it != channels->end())
    {
        if (!it->isInviteOnly())
        {
            it->addMember(_target);
            this->_target->setChannel(&(*it));
        }
        else
        {
            send(this->_target->getFd(), Reply::inviteonlychan(this->_target->getNickname(), it->getName()).c_str(), Reply::inviteonlychan(this->_target->getNickname(), it->getName()).length(), 0);
        }
    }
    else
    {
        Channel newChannel(channelName);
        newChannel.addOperator(_target);
        channels->push_back(newChannel);
        this->_target->setChannel(&channels->back());
    }
}
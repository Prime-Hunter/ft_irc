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
        throw Command::IncorrectArgNumber();
    }
    if (this->_args[1].empty())
    {
        throw Command::EmptyArg();
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
        it->addMember(_target);
    }
    else
    {
        Channel newChannel(channelName);
        newChannel.addOperator(_target);
        channels->push_back(newChannel);
    }
}
#include "../includes/Command.hpp"

void Command::part()
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
        if (it->getName() == channelName)
            break;
    }
    if (it == channels->end())
    {
        throw (Command::InvalidChannel());
    }
    it->removeMember(_target);
    _target->setChannel(Channel());
    if (it->getClientCount() <= 0)
    {
        channels->erase(it);
    }
}
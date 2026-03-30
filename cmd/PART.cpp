#include "../includes/Command.hpp"

void Command::part()
{
    if (_args.empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "PART");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string channelName = _args[1];
    if (channelName.empty() || channelName[0] != '#')
    {
        std::string reply = Reply::nosuchchannel(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    Channel *chan = getChannel(channelName);
    if (!chan)
    {
        std::string reply = Reply::nosuchchannel(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (!chan->isMember(_target))
    {
        std::string reply = Reply::notonchannel(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }

    std::string partMsg = Reply::part(_target->getPrefix(), channelName);
    chan->broadcast(partMsg, _target);
    chan->removeMember(_target);
    _target->setChannel(NULL);
    if (chan->getClientCount() == 0)
    {
        std::vector<Channel> *channels = _serv->getChannels();
        for (std::vector<Channel>::iterator it = channels->begin(); it != channels->end(); ++it)
        {
            if (it->getName() == channelName)
            {
                channels->erase(it);
                break;
            }
        }
    }
}

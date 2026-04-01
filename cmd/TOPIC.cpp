#include "../includes/Command.hpp"

void Command::topic(void)
{
    if (_args.size() < 2 || _args[1].empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "TOPIC");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string channelName = _args[1];
    Channel *chan = getChannel(channelName);
    if (!chan)
    {
        std::string reply = Reply::nosuchchannel(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (_args.size() == 2)
    {
        if (chan->getTopic().empty())
        {
            std::string reply = Reply::notopic(_target->getNickname(), channelName);
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
        }
        else
        {
            std::string reply = Reply::topic(_target->getNickname(), channelName, chan->getTopic());
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
        }
        return;
    }
    if (!chan->isMember(_target))
    {
        std::string reply = Reply::notonchannel(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (chan->isRestricted() && !chan->isOperator(_target))
    {
        std::string reply = Reply::chanoprivsneeded(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string topicText = _args[2];
    chan->setTopic(topicText);
    std::string topicMsg = ":" + _target->getPrefix() + " TOPIC " + channelName + " :" + topicText + "\r\n";
    chan->broadcast(topicMsg, _target);
}


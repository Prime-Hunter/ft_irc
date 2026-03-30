#include "../includes/Command.hpp"

void Command::privmsg(void)
{
    if (_args.size() < 3)
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "PRIVMSG");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string target = _args[1];
    std::string message = _args[2];
    if (target.empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "PRIVMSG");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (message.size() == 0)
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "PRIVMSG");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (message[0] == ':')
        message = message.substr(1);
    std::string fullMsg = Reply::privmsg(_target->getPrefix(), target, message);
    if (target[0] == '#')
    {
        Channel *chan = getChannel(target);
        if (!chan)
        {
            std::string reply = Reply::nosuchchannel(_target->getNickname(), target);
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
            return;
        }
        if (!chan->isMember(_target))
        {
            std::string reply = Reply::notonchannel(_target->getNickname(), target);
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
            return;
        }
        chan->sendMessage(fullMsg, _target);
        return;
    }
    Client *dst = getNClient(target);
    if (!dst)
    {
        std::string reply = Reply::nosuchnick(_target->getNickname(), target);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    send(dst->getFd(), fullMsg.c_str(), fullMsg.length(), 0);
}

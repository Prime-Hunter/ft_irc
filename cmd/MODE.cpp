#include "../includes/Command.hpp"

void Command::mode(void)
{
    if (_args.size() < 2 || _args[1].empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "MODE");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string target = _args[1];
    std::string modes = "";
    if (_args.size() >= 3)
        modes = _args[2];
    if (modes.empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "MODE");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
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

        bool plus = (modes[0] == '+');
        for (size_t i = 1; i < modes.size(); i++)
        {
            if (modes[i] == 'i')
                chan->setInviteOnly(plus ? 1 : 0);
            else if (modes[i] == 't')
                chan->setRestricted(plus ? 1 : 0);
            else
            {
                std::string reply = Reply::unknownmode(_target->getNickname(), std::string(1, modes[i]));
                send(_target->getFd(), reply.c_str(), reply.length(), 0);
                return;
            }
        }
        std::string modeReply = Reply::mode(_target->getPrefix(), target, modes, (_args.size() > 3 ? _args[3] : ""));
        chan->broadcast(modeReply, _target);
        return;
    }
    Client *user = getNClient(target);
    if (!user)
    {
        std::string reply = Reply::nosuchnick(_target->getNickname(), target);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string modeReply = Reply::mode(_target->getPrefix(), target, modes, (_args.size() > 3 ? _args[3] : ""));
    send(_target->getFd(), modeReply.c_str(), modeReply.length(), 0);
}

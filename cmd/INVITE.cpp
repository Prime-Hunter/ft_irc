#include "../includes/Command.hpp"

void Command::invite(void)
{
    if (_args.size() < 3 || _args[1].empty() || _args[2].empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "INVITE");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string inviteeNick = _args[1];
    std::string channelName = _args[2];
    Client *invitee = getNClient(inviteeNick);
    if (!invitee)
    {
        std::string reply = Reply::nosuchnick(_target->getNickname(), inviteeNick);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    Channel *chan = getChannel(channelName);
    if (chan)
    {
        if (!chan->isMember(_target))
        {
            std::string reply = Reply::notonchannel(_target->getNickname(), channelName);
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
            return;
        }
        if (chan->isInviteOnly() && !chan->isOperator(_target))
        {
            std::string reply = Reply::chanoprivsneeded(_target->getNickname(), channelName);
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
            return;
        }
    }
    std::string inviteMsg = ":" + _target->getPrefix() + " INVITE " + inviteeNick + " " + channelName + "\r\n";
    send(_target->getFd(), inviteMsg.c_str(), inviteMsg.length(), 0);
    send(invitee->getFd(), inviteMsg.c_str(), inviteMsg.length(), 0);
}

#include "../includes/Command.hpp"

void Command::join()
{
    if (_args.empty()) {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "JOIN");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string channelName = _args[1];
    if (channelName.empty() || channelName[0] != '#') {
        std::string reply = Reply::nosuchchannel(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    Channel *chan = getChannel(channelName);
    if (!chan) {
        Channel newChan(channelName);
        _serv->getChannels()->push_back(newChan);
        chan = &(_serv->getChannels()->back());
    }
    if (chan->isInviteOnly()) {
        std::string reply = Reply::inviteonlychan(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (chan->isMember(_target)) {
        return;
    }
    if (chan->getClientCount() == 0) {
        chan->addOperator(_target);
    } else {
        chan->addMember(_target);
    }

    std::string joinMsg = Reply::join(_target->getPrefix(), channelName);
    chan->broadcast(joinMsg, _target);

    std::string users = chan->getUsers();
    std::string namreply = Reply::namreply(_target->getPrefix(), channelName, users);
    send(_target->getFd(), namreply.c_str(), namreply.length(), 0);

    std::string endofnames = Reply::endofnames(_target->getPrefix(), channelName);
    send(_target->getFd(), endofnames.c_str(), endofnames.length(), 0);
}
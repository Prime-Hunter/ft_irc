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
    std::string message;
    for (size_t i = 2; i < _args.size(); ++i)
    {
        if (i > 2) message += " ";
        message += _args[i];
    }
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
        _serv->getBot()->checkMsg(message, chan, _target);
        return;
    }
    Client *dst = getNClient(target);
    if (!dst)
    {
        std::string reply = Reply::nosuchnick(_target->getNickname(), target);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (isCtcpMessage(message))
    {
        std::string inner = message.substr(1, message.size() - 2);
        std::istringstream iss(inner);
        std::vector<std::string> dccArgs;
        std::string token;
        while (iss >> token)
            dccArgs.push_back(token);

        if (dccArgs.size() >= 5 && dccArgs[0] == "DCC" && dccArgs[1] == "SEND")
        {
            std::string filename = dccArgs[2];
            std::string senderIp = dccArgs[3];
            int senderPort = std::atoi(dccArgs[4].c_str());
            size_t fileSize = 0;
            if (dccArgs.size() >= 6)
                fileSize = std::strtoul(dccArgs[5].c_str(), NULL, 10);
            _serv->startDccProxy(_target, dst, filename, senderIp, senderPort, fileSize);
            return;
        }

        send(dst->getFd(), fullMsg.c_str(), fullMsg.length(), 0);
        return;
    }
    send(dst->getFd(), fullMsg.c_str(), fullMsg.length(), 0);
}

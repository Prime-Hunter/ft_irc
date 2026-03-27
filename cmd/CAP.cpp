#include "../includes/Command.hpp"

void Command::cap(void)
{
    std::string nick = this->_target->getNickname().empty() ? "*" : this->_target->getNickname();

    if (this->_args.size() < 2)
    {
        std::string message = Reply::needmoreparams(nick, "CAP");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    std::string subcmd = this->_args[1];
    if (subcmd == "LS")
    {
        send(this->_target->getFd(), "CAP * LS :\r\n", sizeof("CAP * LS :\r\n") - 1, 0);
    }
    else if (subcmd == "END")
    {
        return;
    }
    else if (subcmd == "REQ")
    {
        send(this->_target->getFd(), "CAP * NAK :\r\n", sizeof("CAP * NAK :\r\n") - 1, 0);
    }
    else
    {
        send(this->_target->getFd(), "CAP * NAK :\r\n", sizeof("CAP * NAK :\r\n") - 1, 0);
    }
}

#include "../includes/Command.hpp"

void Command::pass(void)
{
    std::string nick = this->_target->getNickname().empty() ? "*" : this->_target->getNickname();

    if (this->_args.size() != 2)
    {
        std::string message = Reply::needmoreparams(nick, "PASS");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (this->_args[1].empty())
    {
        std::string message = Reply::needmoreparams(nick, "PASS");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (this->_target->getLogin())
    {
        std::string message = Reply::alreadyregistered(nick);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (this->_args[1] == *this->_serv->getPword())
    {
        this->_target->logIn();
    }
    else
    {
        std::string message = Reply::passwdmismatch(nick);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
    }
}
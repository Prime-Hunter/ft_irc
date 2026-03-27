#include "../includes/Command.hpp"

void Command::user(void)
{
    std::string nick = this->_target->getNickname().empty() ? "*" : this->_target->getNickname();

    if (this->_args.size() != 5)
    {
        std::string message = Reply::needmoreparams(nick, "USER");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (this->_args[1].empty() || this->_args[2].empty() || this->_args[3].empty() || this->_args[4].empty())
    {
        std::string message = Reply::needmoreparams(nick, "USER");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (!this->_target->getLogin())
    {
        std::string message = Reply::notregistered(nick);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (!this->_target->getUsername().empty())
    {
        std::string message = Reply::alreadyregistered(nick);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    this->_target->setUsername(this->_args[1]);
    this->_target->setHostname(this->_args[2]);
    this->_target->setServername(this->_args[3]);
    this->_target->setRealname(this->_args[4]);
    this->_target->checkAuth();
}
#include "../includes/Command.hpp"

void Command::nick(void)
{
    if (this->_args.size() != 2)
    {
        throw Command::IncorrectArgNumber();
    }
    if (this->_args[1].empty())
    {
        throw Command::EmptyArg();
    }
    if (!this->_target->getLogin())
    {
        throw Command::UserNotLogged();
    }
    if (this->_target->getUsername().empty())
    {
        throw Command::UsernameNotSet();
    }
    if (!this->_target->getNickname().empty())
    {
        throw Command::NicknameSet();
    }
    this->_target->setNickname(this->_args[1]);
    this->_target->checkAuth();
    send(this->_target->getFd(), "Your nickname is set !\n", sizeof("Your nickname is set !\n"), 0);
}
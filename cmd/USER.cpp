#include "../includes/Command.hpp"

void Command::user(void)
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
    if (!this->_target->getUsername().empty())
    {
        throw Command::UsernameSet();
    }
    this->_target->setUsername(this->_args[1]);
    send(this->_target->getFd(), "Your username is set !\n", sizeof("Your username is set !\n"), 0);
}
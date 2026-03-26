#include "../includes/Command.hpp"

void Command::user(void)
{
    if (this->_args.size() != 5)
    {
        throw Command::IncorrectArgNumber();
    }
    if (this->_args[1].empty() || this->_args[2].empty() || this->_args[3].empty() || this->_args[4].empty())
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
    this->_target->setHostname(this->_args[2]);
    this->_target->setServername(this->_args[3]);
    this->_target->setRealname(this->_args[4]);
    this->_target->checkAuth();
}
#include "../includes/Command.hpp"

void Command::pass(void)
{
    if (this->_args.size() != 2)
    {
        throw Command::IncorrectArgNumber();
    }
    if (this->_args[1].empty())
    {
        throw Command::EmptyArg();
    }
    if (this->_target->getLogin())
    {
        throw Command::UserAlreadyLogged();
    }
    if (this->_args[1] == *this->_serv->getPword())
    {
        this->_target->logIn();
    }
    else
    {
        throw Command::IncorrectPassword();
    }
}
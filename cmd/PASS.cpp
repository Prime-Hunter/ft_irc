#include "../includes/Command.hpp"

void Command::pass(void)
{
    if (this->_args.size() != 2)
    {
        return ;
    }
    if (this->_args[1].empty())
    {
        return ;
    }
    if (target.getLogin())
    {
        return ;
    }
    if (this->_args[1].compare(this->_serv;getPword()))
    {
        this->_target.logIn();
    }
    else
    {
        return ;
    }
}
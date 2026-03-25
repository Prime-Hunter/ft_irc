#include "../includes/Command.hpp"

Command::Command() {}
Command::~Command() {}
Command::Command(std::string name, std::vector<std::string> args, Client *target, Server *serv): _name(name), _args(args), _target(target), _serv(serv) {};

void Command::displayCmd()
{
    std::cout << "NAME: " << this->_name << std::endl;
    std::cout << "TARGET: " << this->_target << std::endl;
    std::cout << "ARGS: ";
    for (std::vector<std::string>::iterator i=this->_args.begin(); i!=this->_args.end(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}

void Command::execCmd(void)
{
    if (!this->_name.compare("/pass"))
    {
        this->pass();
    }
    else if (!this->_name.compare("/user"))
    {
        this->user();
    }
    else if (!this->_name.compare("/nick"))
    {
        this->nick();
    }
    else if (!this->_name.compare("/join"))
    {
        this->join();
    }
    else
    {
        throw Command::UnknownCmd();
    }
}

const char *Command::UnknownCmd::what(void) const throw()
{
    return ("Command Unknown");
};

const char *Command::IncorrectArgNumber::what(void) const throw()
{
    return ("Incorrect Argument Number");
};

const char *Command::EmptyArg::what(void) const throw()
{
    return ("Empty Argument");
};

const char *Command::UserNotLogged::what(void) const throw()
{
    return ("You have not enter password yet");
};

const char *Command::UserAlreadyLogged::what(void) const throw()
{
    return ("You are already logged in");
};

const char *Command::UsernameNotSet::what(void) const throw()
{
    return ("You have not set your username yet");
};

const char *Command::UsernameSet::what(void) const throw()
{
    return ("Username Already Set");
};

const char *Command::NicknameSet::what(void) const throw()
{
    return ("Nickname Already Set");
};

const char *Command::IncorrectPassword::what(void) const throw()
{
    return ("Incorrect Password");
};
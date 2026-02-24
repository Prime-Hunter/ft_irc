#include "../includes/Command.hpp"

Command::Command() {}
Command::~Command() {}
Command::Command(std::string name, std::vector<std::string> args, int target, Server *serv): _name(name), _args(args), _targetfd(target), _serv(serv) {};

void Command::displayCmd()
{
    std::cout << "NAME: " << this->_name << std::endl;
    std::cout << "TARGET: " << this->_targetfd << std::endl;
    std::cout << "ARGS: ";
    for (std::vector<std::string>::iterator i=this->_args.begin(); i!=this->_args.end(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}

void Command::execCmd()
{
    
}
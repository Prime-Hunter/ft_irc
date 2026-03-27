#include "../includes/Command.hpp"

Command::Command() {}
Command::~Command() {}
Command::Command(std::string name, std::vector<std::string> args, Client *target, Server *serv): _name(name), _args(args), _target(target), _serv(serv) {};

void Command::displayCmd()
{
    std::cout << "NAME: " << this->_name << std::endl;
    std::cout << "TARGET: " << this->_target->getUsername() << std::endl;
    std::cout << "ARGS: ";
    for (std::vector<std::string>::iterator i=this->_args.begin(); i!=this->_args.end(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}

int Command::isAvailableNickname(const std::string &nick)
{
    std::vector<Client> *list = this->getServ()->getList();
    for (std::vector<Client>::iterator it = list->begin(); it != list->end(); ++it)
    {
        if (it->getNickname() == nick) {return 0;}
    }
    return 1;
}

Channel *Command::getChannel(const std::string &name)
{
    std::vector<Channel> *list = this->getServ()->getChannels();
    for (std::vector<Channel>::iterator it = list->begin(); it != list->end(); ++it)
    {
        if (it->getName() == name)
            return &(*it);
    }
    return NULL;
}

Client *Command::getNClient(const std::string &name)
{
    std::vector<Client> *list = this->getServ()->getList();
    for (std::vector<Client>::iterator it = list->begin(); it != list->end(); ++it)
    {
        if (it->getNickname() == name)
            return &(*it);
    }
    return NULL;
}

void Command::execCmd(void)
{
    if (!this->_name.compare("PASS"))
    {
        this->pass();
    }
    else if (!this->_name.compare("USER"))
    {
        this->user();
    }
    else if (!this->_name.compare("NICK"))
    {
        this->nick();
    }
    else if (!this->_name.compare("JOIN"))
    {
        this->join();
    }
    else if (!this->_name.compare("PART"))
    {
        this->part();
    }
    else if (!this->_name.compare("CAP"))
    {
        this->cap();
    }
    else if (!this->_name.compare("KICK"))
    {
        this->kick();
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

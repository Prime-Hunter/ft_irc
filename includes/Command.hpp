#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"

class Command
{
    private:
        std::string _name;
        std::vector<std::string> _args;
        int _targetfd;
        std::string *_password;
        std::vector<Client> *_clientList;

    public:
        Command();
        Command(std::string name, std::vector<std::string> args, int targetfd, std::string *pword, std::vector<Client> *list);
        ~Command();

        void displayCmd();
        void execCmd();

        void pass();
};

#endif
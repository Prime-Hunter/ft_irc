#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"

class Command
{
    private:
        std::string _name;
        std::vector<std::string> _args;
        int _targetfd;
        Server *_serv;

    public:
        Command();
        Command(std::string name, std::vector<std::string> args, int targetfd, Server *serv);
        ~Command();

        void displayCmd(void);
        void execCmd(void);

        void pass(void);
};

#endif
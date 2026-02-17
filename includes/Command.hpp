#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"

class Command
{
    private:
        std::string _name;
        std::vector<std::string> _args;
        int _targetfd;

    public:
        Command();
        Command(std::string name, std::vector<std::string> args, int targetfd);
        ~Command();

        void displayCmd();
        void execCmd();

        void pass();
};

#endif
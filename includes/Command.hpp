#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Replies.hpp"

class Command
{
    private:
        std::string _name;
        std::vector<std::string> _args;
        Client *_target;
        Server *_serv;

    public:
        Command();
        Command(std::string name, std::vector<std::string> args, Client *target, Server *serv);
        ~Command();

        void displayCmd(void);
        void execCmd(void);
        int isAvailableNickname(const std::string &nick);
        Server *getServ() {return this->_serv;}
        Channel *getChannel(const std::string &name);
        Client *getNClient(const std::string &nickname);

        void pass(void);
        void user(void);
        void nick(void);
        void join(void);
        void part(void);
        void cap(void);
        void kick(void);
        void privmsg(void);

    class UnknownCmd: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

};

#endif
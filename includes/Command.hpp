#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"

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

        void pass(void);
        void user(void);
        void nick(void);

    class UnknownCmd: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

    class IncorrectArgNumber: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

    class EmptyArg: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

    class UserNotLogged: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

    class UserAlreadyLogged: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

    class UsernameNotSet: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

    class UsernameSet: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

    class NicknameSet: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };

    class IncorrectPassword: public std::exception
    {
        public:
            virtual const char *what() const throw();
    };
};

#endif
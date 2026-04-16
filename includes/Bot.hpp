#ifndef BOT_HPP
#define BOT_HPP

#include "Channel.hpp"
#include "Client.hpp"

#include <map>
#include <set>


class Bot
{
    private:
        std::set<std::string> _wordList;
        std::map<Client *, int> _warnList;
    
    public:
        Bot();
        Bot(std::set<std::string> wordList);
        ~Bot();
        Bot(Bot const &copy);
	    Bot &operator=(Bot const &src);

        void checkMsg(std::string msg, Channel *channel, Client *client);
        void warnClient(Client *client, Channel *channel);
        void kickClient(Client *client, Channel *channel);

};

#endif
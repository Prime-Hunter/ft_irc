#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>
#include <sstream>

class Reply
{
    public:
        //Message replies
        static std::string welcome(const std::string &nick, const std::string &user, const std::string &host) {return ":localhost 001 " + nick + " :Welcome to the IRC Network " + nick + "!" + user + "@" + host + "\r\n";}
        static std::string yourhost(const std::string &nick) {return ":localhost 002 " + nick + " :Your host is localhost, running version 1.0\r\n";}
        static std::string created(const std::string &nick) {return ":localhost 003 " + nick + " :This server was created now\r\n";}
        static std::string myinfo(const std::string &nick) {return ":localhost 004 " + nick + " localhost 1.0 o o\r\n";}
        static std::string namreply(const std::string &nick, const std::string &channel, const std::string &users) {return ":localhost 353 " + nick + " = " + channel + " :" + users + "\r\n";}
        static std::string endofnames(const std::string &nick, const std::string &channel) {return ":localhost 366 " + nick + " " + channel + " :End of NAMES list\r\n";}
        static std::string notopic(const std::string &nick, const std::string &channel) {return ":localhost 331 " + nick + " " + channel + " :No topic is set\r\n";}
        static std::string topic(const std::string &nick, const std::string &channel, const std::string &topic_text) {return ":localhost 332 " + nick + " " + channel + " :" + topic_text + "\r\n";}
        static std::string topicwhotime(const std::string &nick, const std::string &channel, const std::string &who, const std::string &time) {return ":localhost 333 " + nick + " " + channel + " " + who + " " + time + "\r\n";}

        //Error replies
        static std::string nonicknamegiven(const std::string &nick) {return ":localhost 431 " + nick + " :No nickname given\r\n";}
        static std::string erroneusnickname(const std::string &nick) {return ":localhost 432 " + nick + " :Erroneous nickname\r\n";}
        static std::string nicknameinuse(const std::string &nick) {return ":localhost 433 * " + nick + " :Nickname is already in use\r\n";}
        static std::string nosuchnick(const std::string &nick, const std::string &target) {return ":localhost 401 " + nick + " " + target + " :No such nick/channel\r\n";}
        static std::string nosuchchannel(const std::string &nick, const std::string &channel) {return ":localhost 403 " + nick + " " + channel + " :No such channel\r\n";}
        static std::string needmoreparams(const std::string &nick, const std::string &command) {return ":localhost 461 " + nick + " " + command + " :Not enough parameters\r\n";}
        static std::string alreadyregistered(const std::string &nick) {return ":localhost 462 " + nick + " :You may not reregister\r\n";}
        static std::string notonchannel(const std::string &nick, const std::string &channel) {return ":localhost 442 " + nick + " " + channel + " :You're not on that channel\r\n";}
        static std::string channelisfull(const std::string &nick, const std::string &channel) {return ":localhost 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";}
        static std::string unknownmode(const std::string &nick, const std::string &mode) {return ":localhost 472 " + nick + " " + mode + " :is unknown mode char to me\r\n";}
        static std::string notregistered(const std::string &nick) {return ":localhost 451 " + nick + " :You have not registered\r\n";}
        static std::string passwdmismatch(const std::string &nick) {return ":localhost 464 " + nick + " :Password incorrect\r\n";}
        static std::string inviteonlychan(const std::string &nick, const std::string &channel) {return ":localhost 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";}
        static std::string bannedfromchan(const std::string &nick, const std::string &channel) {return ":localhost 474 " + nick + " " + channel + " :Cannot join channel (+b)\r\n";}
        static std::string badchannelkey(const std::string &nick, const std::string &channel) {return ":localhost 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";}
        static std::string chanoprivsneeded(const std::string &nick, const std::string &channel) {return ":localhost 482 " + nick + " " + channel + " :You're not channel operator\r\n";}

        //Command replies
        static std::string join(const std::string &nick, const std::string &channel) {return ":" + nick + " JOIN :" + channel + "\r\n";}
        static std::string part(const std::string &nick, const std::string &channel) {return ":" + nick + " PART :" + channel + "\r\n";}
        static std::string ping(const std::string &nick, const std::string &token) {return ":" + nick + " PONG:" + token + "\r\n";}
        static std::string privmsg(const std::string &nick, const std::string &channel, const std::string &message) {return ":" + nick + " PRIVMSG " + channel + " :" + message + "\r\n";}
        static std::string quit(const std::string &nick, const std::string &message) {return ":" + nick + " QUIT :Quit: " + message + "\r\n";}
        static std::string kick(const std::string &nick, const std::string &channel, const std::string &target, const std::string &reason) {return ":" + nick + " KICK " + channel + " " + target + " :" + reason + "\r\n";}
        static std::string mode(const std::string &nick, const std::string &channel, const std::string &modes, const std::string &args) {return ":" + nick + " MODE " + channel + " " + modes + " " + args + "\r\n";}
};

#endif
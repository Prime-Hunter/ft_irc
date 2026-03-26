#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>
#include <sstream>

// IRC Reply Codes
namespace IRC
{
    // Numeric Replies
    const int RPL_WELCOME = 001;
    const int RPL_YOURHOST = 002;
    const int RPL_CREATED = 003;
    const int RPL_MYINFO = 004;
    const int RPL_BOUNCE = 005;

    const int RPL_LUSERCLIENT = 251;
    const int RPL_LUSEROP = 252;
    const int RPL_LUSERUNKNOWN = 253;
    const int RPL_LUSERCHANNELS = 254;
    const int RPL_LUSERME = 255;
    const int RPL_LOCALUSERS = 265;
    const int RPL_GLOBALUSERS = 266;

    const int RPL_NOTOPIC = 331;
    const int RPL_TOPIC = 332;
    const int RPL_TOPICWHOTIME = 333;
    const int RPL_NAMREPLY = 353;
    const int RPL_ENDOFNAMES = 366;

    const int RPL_MOTDSTART = 375;
    const int RPL_MOTD = 372;
    const int RPL_ENDOFMOTD = 376;

    // Error Codes
    const int ERR_NOSUCHNICK = 401;
    const int ERR_NOSUCHCHANNEL = 403;
    const int ERR_CANNOTSENDTOCHAN = 404;
    const int ERR_TOOMANYCHANNELS = 405;
    const int ERR_NONICKNAMEGIVEN = 431;
    const int ERR_ERRONEUSNICKNAME = 432;
    const int ERR_NICKNAMEINUSE = 433;
    const int ERR_NICKCOLLISION = 436;
    const int ERR_NOTONCHANNEL = 442;
    const int ERR_NEEDMOREPARAMS = 461;
    const int ERR_ALREADYREGISTERED = 462;
    const int ERR_CHANNELISFULL = 471;
    const int ERR_UNKNOWNMODE = 472;
    const int ERR_INVITEONLYCHAN = 473;
    const int ERR_BANNEDFROMCHAN = 474;
    const int ERR_BADCHANNELKEY = 475;
    const int ERR_CHANOPRIVSNEEDED = 482;

    // Helper functions to build IRC replies
    class Reply
    {
        public:
            static std::string numeric(int code, const std::string &nick, const std::string &params)
            {
                std::ostringstream oss;
                oss << ":" << "localhost" << " ";
                if (code < 10)
                    oss << "00";
                else if (code < 100)
                    oss << "0";
                oss << code << " " << nick << " " << params << "\r\n";
                return oss.str();
            }

            static std::string welcome(const std::string &nick, const std::string &user, const std::string &host)
            {
                std::ostringstream oss;
                oss << ":localhost 001 " << nick << " :Welcome to the IRC Network " << nick << "!" << user << "@" << host << "\r\n";
                return oss.str();
            }

            static std::string yourhost(const std::string &nick) {return ":localhost 002 " + nick + " :Your host is localhost, running version 1.0\r\n";}

            static std::string created(const std::string &nick) {return ":localhost 003 " + nick + " :This server was created now\r\n";}

            static std::string myinfo(const std::string &nick) {return ":localhost 004 " + nick + " localhost 1.0 o o\r\n";}

            static std::string namreply(const std::string &nick, const std::string &channel, const std::string &users) {return ":localhost 353 " + nick + " = " + channel + " :" + users + "\r\n";}

            static std::string endofnames(const std::string &nick, const std::string &channel) {return ":localhost 366 " + nick + " " + channel + " :End of NAMES list\r\n";}

            static std::string notopic(const std::string &nick, const std::string &channel) {return ":localhost 331 " + nick + " " + channel + " :No topic is set\r\n";}

            static std::string topic(const std::string &nick, const std::string &channel, const std::string &topic_text) {return ":localhost 332 " + nick + " " + channel + " :" + topic_text + "\r\n";}

            static std::string topicwhotime(const std::string &nick, const std::string &channel, const std::string &who, const std::string &time) {return ":localhost 333 " + nick + " " + channel + " " + who + " " + time + "\r\n";}

            static std::string nicknameinuse(const std::string &nick) {return ":localhost 433 * " + nick + " :Nickname is already in use\r\n";}

            static std::string nosuchnick(const std::string &nick, const std::string &target) {return ":localhost 401 " + nick + " " + target + " :No such nick/channel\r\n";}

            static std::string nosuchchannel(const std::string &nick, const std::string &channel) {return ":localhost 403 " + nick + " " + channel + " :No such channel\r\n";}

            static std::string needmoreparams(const std::string &nick, const std::string &command) {return ":localhost 461 " + nick + " " + command + " :Not enough parameters\r\n";}

            static std::string alreadyregistered(const std::string &nick) {return ":localhost 462 " + nick + " :You may not reregister\r\n";}

            static std::string notonchannel(const std::string &nick, const std::string &channel) {return ":localhost 442 " + nick + " " + channel + " :You're not on that channel\r\n";}

            static std::string channelisfull(const std::string &nick, const std::string &channel) {return ":localhost 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";}

            static std::string unknownmode(const std::string &nick, const std::string &mode) {return ":localhost 472 " + nick + " " + mode + " :is unknown mode char to me\r\n";}

            static std::string inviteonlychan(const std::string &nick, const std::string &channel) {return ":localhost 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";}

            static std::string bannedfromchan(const std::string &nick, const std::string &channel) {return ":localhost 474 " + nick + " " + channel + " :Cannot join channel (+b)\r\n";}

            static std::string badchannelkey(const std::string &nick, const std::string &channel) {return ":localhost 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";}

            static std::string chanoprivsneeded(const std::string &nick, const std::string &channel) {return ":localhost 482 " + nick + " " + channel + " :You're not channel operator\r\n";}
    };
}

#endif
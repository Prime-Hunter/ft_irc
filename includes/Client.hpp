#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include "Channel.hpp"

class Client
{
    private:
        int _fd;
        int _login;
        int _write;
        std::string _ipAddress;
        std::string _username;
        std::string _hostname;
        std::string _servername;
        std::string _realname;
        std::string _nickname;
        std::string _tempBuffer;
        Channel *_currentChannel;
    
    public:
        Client(void);
        Client(int fd, std::string username, std::string nickname);
        ~Client(void);
        Client(const Client &copy);
        Client &operator =(const Client &src);

        int getFd(void);
        int getLogin(void);
        std::string getUsername(void);
        std::string getNickname(void);
        std::string getHostname(void);
        std::string getServername(void);
        std::string getRealname(void);
        std::string getPrefix(void);
        int getWrite(void);
        Channel getChannel(void);

        void setFd(int fd);
        void setIpAddress(std::string ipAddress);
        void setUsername(std::string username);
        void setNickname(std::string username);
        void setHostname(std::string hostname);
        void setServername(std::string servername);
        void setRealname(std::string realname);
        void setChannel(Channel *newChannel);
        void appendTempBuffer(const std::string &data);
        std::string getTempBuffer(void);
        void clearTempBuffer(void);
        void logIn(void);
        void checkAuth(void);
        bool isRegistered(void);
        bool hasChannel(void);
};

#endif
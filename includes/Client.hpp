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

class Client
{
    private:
        int _fd;
        std::string _ipAddress;
        std::string _username;
        std::string _nickname;
    
    public:
        Client(void);
        Client(int fd, std::string username, std::string nickname);
        ~Client(void);
        Client(const Client &copy);
        Client &operator =(const Client &src);

        int getFd(void);
        std::string getUsername(void);
        std::string getNickname(void);

        void setFd(int fd);
        void setIpAddress(std::string ipAddress);
        void setUsername(std::string username);
        void setNickname(std::string username);
};

#endif
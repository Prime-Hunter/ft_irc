#pragma once 

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
#include <string.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <list>
#include <netdb.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "Bot.hpp"

class Channel;

#define BUFFER_SIZE 1024

class Server
{
    private:
        int _port;
        std::string _password;
        int _socketFd;
        static bool _signal;
        std::list<Client> _clientList;
        std::vector<struct pollfd> _fds;
        std::vector<Channel> _channelList;
        Bot *_bot;

        struct DccProxy
        {
            int proxyListenFd;
            int proxyConnectionFd;
            int senderDataFd;
            int senderClientFd;
            int receiverClientFd;
            std::string senderIp;
            int senderPort;
            std::string filename;
            size_t fileSize;
            int proxyPort;
            bool receiverConnected;
            bool senderConnected;
            std::string senderPending;
            std::string receiverPending;
            DccProxy(): proxyListenFd(-1), proxyConnectionFd(-1), senderDataFd(-1), senderClientFd(-1), receiverClientFd(-1), senderPort(0), fileSize(0), proxyPort(0), receiverConnected(false), senderConnected(false) {}
        };

        std::vector<DccProxy> _dccProxies;

    public:
        Server();
        ~Server();
        Server(Server const &copy);
	    Server &operator=(Server const &src);

        void serverInit(int port, std::string password);
        void createSocket();

        void acceptClient();
        void newClientData(int clientFd);

        static void handleSignal(int signum);

        void closeFds();
        void clearClient(int clientFd);
        Client *getClient(int fd);

        std::string *getPword(void);
        std::list<Client> *getList(void);
        std::vector<Channel> *getChannels(void);
        Bot *getBot(void);

        void addPollFd(int fd, short events);
        void removePollFd(int fd);
        void updatePollFd(int fd, short events);
        DccProxy *getDccProxyByFd(int fd);
        DccProxy *getDccProxyByListenFd(int fd);
        DccProxy *getDccProxyBySenderReceiver(int senderFd, int receiverFd, const std::string &filename);
        DccProxy *getDccProxyBySenderAndName(int senderFd, const std::string &filename);
        void removeDccProxy(int index);
        void handleDccEvent(int fd, short revents);
        void startDccProxy(Client *sender, Client *receiver, const std::string &filename, const std::string &senderIp, int senderPort, size_t fileSize);
        std::string getServerIp(void);

};

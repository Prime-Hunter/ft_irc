#include "../includes/Server.hpp"
#include "../includes/Command.hpp"

bool Server::_signal = false;

Server::Server(void) 
{
    this->_socketFd = -1;
}

Server::~Server()
{
    delete this->_bot;
}

Server::Server(Server const &copy)
{
    *this = copy;
}

Server &Server::operator=(Server const &src){
	if (this == &src)
    {
		return (*this);
    }
	this->_port = src._port;
	this->_socketFd = src._socketFd;
	this->_clientList = src._clientList;
	this->_fds = src._fds;
	return (*this);
}

std::string *Server::getPword(void)
{
    return (&this->_password);
}

std::list<Client> *Server::getList(void)
{
    return (&this->_clientList);
}

void Server::clearClient(int clientFd)
{
	for (size_t i = 0; i < this->_fds.size(); i++)
    {
		if (this->_fds[i].fd == clientFd)
		{
            this->_fds.erase(this->_fds.begin() + i); 
            break;
        }
	}
	for (std::list<Client>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); ++it)
    {
		if (it->getFd() == clientFd)
		{
            this->_clientList.erase(it);
            break;
        }
	}

}

void Server::closeFds(void)
{
    for (std::list<Client>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); ++it)
    {
        std::cout << it->getFd() << " disconnected" << std::endl;
        close(it->getFd());
    }
    if (this->_socketFd != -1)
    {
        std::cout << "Server " << this->_socketFd << " disconnected" << std::endl;
        close(this->_socketFd);
    }
    for (std::vector<DccProxy>::iterator it = this->_dccProxies.begin(); it != this->_dccProxies.end(); ++it)
    {
        if (it->proxyListenFd != -1)
            close(it->proxyListenFd);
        if (it->proxyConnectionFd != -1)
            close(it->proxyConnectionFd);
        if (it->senderDataFd != -1)
            close(it->senderDataFd);
    }
}

void Server::addPollFd(int fd, short events)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = events;
    pfd.revents = 0;
    this->_fds.push_back(pfd);
}

void Server::removePollFd(int fd)
{
    for (size_t i = 0; i < this->_fds.size(); ++i)
    {
        if (this->_fds[i].fd == fd)
        {
            this->_fds.erase(this->_fds.begin() + i);
            return;
        }
    }
}

void Server::updatePollFd(int fd, short events)
{
    for (size_t i = 0; i < this->_fds.size(); ++i)
    {
        if (this->_fds[i].fd == fd)
        {
            this->_fds[i].events = events;
            return;
        }
    }
}

Server::DccProxy *Server::getDccProxyByFd(int fd)
{
    for (std::vector<DccProxy>::iterator it = this->_dccProxies.begin(); it != this->_dccProxies.end(); ++it)
    {
        if (it->proxyListenFd == fd || it->proxyConnectionFd == fd || it->senderDataFd == fd)
            return &(*it);
    }
    return NULL;
}

Server::DccProxy *Server::getDccProxyByListenFd(int fd)
{
    for (std::vector<DccProxy>::iterator it = this->_dccProxies.begin(); it != this->_dccProxies.end(); ++it)
    {
        if (it->proxyListenFd == fd)
            return &(*it);
    }
    return NULL;
}

Server::DccProxy *Server::getDccProxyBySenderAndName(int senderFd, const std::string &filename)
{
    for (std::vector<DccProxy>::iterator it = this->_dccProxies.begin(); it != this->_dccProxies.end(); ++it)
    {
        if (it->senderClientFd == senderFd && it->filename == filename)
            return &(*it);
    }
    return NULL;
}

void Server::removeDccProxy(int index)
{
    if (index < 0 || static_cast<size_t>(index) >= this->_dccProxies.size())
        return;

    DccProxy &proxy = this->_dccProxies[index];
    if (proxy.proxyListenFd != -1)
    {
        close(proxy.proxyListenFd);
        removePollFd(proxy.proxyListenFd);
    }
    if (proxy.proxyConnectionFd != -1)
    {
        close(proxy.proxyConnectionFd);
        removePollFd(proxy.proxyConnectionFd);
    }
    if (proxy.senderDataFd != -1)
    {
        close(proxy.senderDataFd);
        removePollFd(proxy.senderDataFd);
    }
    this->_dccProxies.erase(this->_dccProxies.begin() + index);
}

std::string Server::getServerIp(void)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        struct hostent *host = gethostbyname(hostname);
        if (host && host->h_addr_list && host->h_addr_list[0])
        {
            struct in_addr addr;
            memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
            return std::string(inet_ntoa(addr));
        }
    }
    return std::string("127.0.0.1");
}

void Server::startDccProxy(Client *sender, Client *receiver, const std::string &filename, const std::string &senderIp, int senderPort, size_t fileSize)
{
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == -1)
    {
        std::cerr << "Failed to create DCC proxy socket" << std::endl;
        return;
    }
    int val = 1;
    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
    {
        close(listenFd);
        std::cerr << "Failed to set SO_REUSEADDR for DCC proxy" << std::endl;
        return;
    }
    if (fcntl(listenFd, F_SETFL, O_NONBLOCK) == -1)
    {
        close(listenFd);
        std::cerr << "Failed to set nonblocking on DCC proxy" << std::endl;
        return;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listenFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        close(listenFd);
        std::cerr << "Failed to bind DCC proxy socket" << std::endl;
        return;
    }
    if (listen(listenFd, 1) == -1)
    {
        close(listenFd);
        std::cerr << "Failed to listen on DCC proxy socket" << std::endl;
        return;
    }
    socklen_t addrlen = sizeof(addr);
    if (getsockname(listenFd, (struct sockaddr *)&addr, &addrlen) == -1)
    {
        close(listenFd);
        std::cerr << "Failed to query DCC proxy port" << std::endl;
        return;
    }

    DccProxy proxy;
    proxy.proxyListenFd = listenFd;
    proxy.senderDataFd = -1;
    proxy.senderClientFd = sender->getFd();
    proxy.receiverClientFd = receiver->getFd();
    proxy.senderIp = senderIp;
    proxy.senderPort = senderPort;
    proxy.filename = filename;
    proxy.fileSize = fileSize;
    proxy.proxyPort = ntohs(addr.sin_port);
    proxy.receiverConnected = false;
    proxy.senderConnected = false;
    this->_dccProxies.push_back(proxy);
    this->addPollFd(listenFd, POLLIN);

    std::string serverIp = this->getServerIp();
    std::ostringstream oss;
    oss << "\001DCC SEND " << filename << " " << serverIp << " " << proxy.proxyPort << " " << fileSize << "\001";
    std::string dccMsg = oss.str();
    std::string fullMsg = Reply::privmsg(sender->getPrefix(), receiver->getNickname(), dccMsg);
    send(receiver->getFd(), fullMsg.c_str(), fullMsg.length(), 0);
}

static bool isNumericIp(const std::string &ip)
{
    for (size_t i = 0; i < ip.size(); ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(ip[i])))
            return false;
    }
    return true;
}

static ssize_t flushPending(int fd, std::string &pending)
{
    if (pending.empty())
        return 0;
    ssize_t sent = send(fd, pending.c_str(), pending.size(), 0);
    if (sent > 0)
        pending.erase(0, sent);
    return sent;
}

void Server::handleDccEvent(int fd, short revents)
{
    DccProxy *proxy = this->getDccProxyByFd(fd);
    if (!proxy)
        return;

    if (proxy->proxyListenFd == fd)
    {
        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);
        int clientFd = accept(fd, (struct sockaddr *)&clientAddr, &len);
        if (clientFd == -1)
            return;
        if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
        {
            close(clientFd);
            return;
        }
        proxy->proxyConnectionFd = clientFd;
        proxy->receiverConnected = true;
        this->addPollFd(clientFd, POLLIN);

        int senderFd = socket(AF_INET, SOCK_STREAM, 0);
        if (senderFd == -1)
        {
            std::cerr << "DCC proxy: failed to create sender socket" << std::endl;
            removeDccProxy(proxy - &this->_dccProxies[0]);
            return;
        }
        if (fcntl(senderFd, F_SETFL, O_NONBLOCK) == -1)
        {
            close(senderFd);
            std::cerr << "DCC proxy: failed to set sender socket nonblocking" << std::endl;
            removeDccProxy(proxy - &this->_dccProxies[0]);
            return;
        }

        struct sockaddr_in senderAddr;
        memset(&senderAddr, 0, sizeof(senderAddr));
        senderAddr.sin_family = AF_INET;
        senderAddr.sin_port = htons(proxy->senderPort);
        if (isNumericIp(proxy->senderIp))
        {
            senderAddr.sin_addr.s_addr = static_cast<in_addr_t>(std::strtoul(proxy->senderIp.c_str(), NULL, 10));
        }
        else
        {
            if (inet_aton(proxy->senderIp.c_str(), &senderAddr.sin_addr) == 0)
            {
                std::cerr << "DCC proxy: invalid sender IP " << proxy->senderIp << std::endl;
                close(senderFd);
                removeDccProxy(proxy - &this->_dccProxies[0]);
                return;
            }
        }

        int connectResult = connect(senderFd, (struct sockaddr *)&senderAddr, sizeof(senderAddr));
        if (connectResult == -1 && errno != EINPROGRESS)
        {
            std::cerr << "DCC proxy: unable to connect to sender " << proxy->senderIp << ":" << proxy->senderPort << std::endl;
            close(senderFd);
            removeDccProxy(proxy - &this->_dccProxies[0]);
            return;
        }
        proxy->senderDataFd = senderFd;
        proxy->senderConnected = (connectResult == 0);
        short events = proxy->senderConnected ? POLLIN : POLLOUT;
        this->addPollFd(senderFd, events);

        std::cout << "DCC proxy: receiver connected on port " << proxy->proxyPort << ". bridging to sender " << proxy->senderIp << ":" << proxy->senderPort << std::endl;
        return;
    }

    if (proxy->senderDataFd == fd)
    {
        if (!proxy->senderConnected)
        {
            if (revents & POLLOUT)
            {
                int err = 0;
                socklen_t len = sizeof(err);
                if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &len) == -1 || err != 0)
                {
                    std::cerr << "DCC proxy: sender connection failed" << std::endl;
                    removeDccProxy(proxy - &this->_dccProxies[0]);
                    return;
                }
                proxy->senderConnected = true;
                short events = POLLIN;
                if (!proxy->receiverPending.empty())
                    events |= POLLOUT;
                this->updatePollFd(fd, events);
            }
            return;
        }

        if ((revents & POLLIN) && proxy->proxyConnectionFd != -1)
        {
            char buffer[BUFFER_SIZE];
            ssize_t count = recv(fd, buffer, sizeof(buffer), 0);
            if (count <= 0)
            {
                removeDccProxy(proxy - &this->_dccProxies[0]);
                return;
            }
            ssize_t sent = send(proxy->proxyConnectionFd, buffer, count, 0);
            if (sent < 0)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    removeDccProxy(proxy - &this->_dccProxies[0]);
                    return;
                }
                proxy->senderPending.append(buffer, count);
                this->updatePollFd(proxy->proxyConnectionFd, POLLIN | POLLOUT);
            }
            else if (static_cast<size_t>(sent) < static_cast<size_t>(count))
            {
                proxy->senderPending.append(buffer + sent, count - sent);
                this->updatePollFd(proxy->proxyConnectionFd, POLLIN | POLLOUT);
            }
        }

        if ((revents & POLLOUT) && !proxy->senderPending.empty() && proxy->proxyConnectionFd != -1)
        {
            ssize_t sent = flushPending(proxy->proxyConnectionFd, proxy->senderPending);
            if (sent < 0)
            {
                removeDccProxy(proxy - &this->_dccProxies[0]);
                return;
            }
            if (proxy->senderPending.empty())
                this->updatePollFd(proxy->proxyConnectionFd, POLLIN);
        }
        return;
    }

    if (proxy->proxyConnectionFd == fd)
    {
        if (revents & POLLIN)
        {
            char buffer[BUFFER_SIZE];
            ssize_t count = recv(fd, buffer, sizeof(buffer), 0);
            if (count <= 0)
            {
                removeDccProxy(proxy - &this->_dccProxies[0]);
                return;
            }
            if (proxy->senderDataFd == -1 || !proxy->senderConnected)
            {
                proxy->receiverPending.append(buffer, count);
                if (proxy->senderDataFd != -1)
                    this->updatePollFd(proxy->senderDataFd, POLLIN | POLLOUT);
            }
            else
            {
                ssize_t sent = send(proxy->senderDataFd, buffer, count, 0);
                if (sent < 0)
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        removeDccProxy(proxy - &this->_dccProxies[0]);
                        return;
                    }
                    proxy->receiverPending.append(buffer, count);
                    this->updatePollFd(proxy->senderDataFd, POLLIN | POLLOUT);
                }
                else if (static_cast<size_t>(sent) < static_cast<size_t>(count))
                {
                    proxy->receiverPending.append(buffer + sent, count - sent);
                    this->updatePollFd(proxy->senderDataFd, POLLIN | POLLOUT);
                }
            }
        }

        if ((revents & POLLOUT) && !proxy->senderPending.empty())
        {
            ssize_t sent = flushPending(fd, proxy->senderPending);
            if (sent < 0)
            {
                removeDccProxy(proxy - &this->_dccProxies[0]);
                return;
            }
            if (proxy->senderPending.empty())
                this->updatePollFd(fd, POLLIN);
        }
        return;
    }
}

void Server::handleSignal(int signum)
{
	(void)signum;
	Server::_signal = true;
}

void Server::createSocket(void)
{
	struct sockaddr_in address;
	struct pollfd newPoll;
	address.sin_family = AF_INET;
	address.sin_port = htons(this->_port);
	address.sin_addr.s_addr = INADDR_ANY;

	this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFd == -1)
	{
		throw(std::runtime_error("Unable to create socket"));
	}

    int val = 1;
    if (setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
    {
        throw(std::runtime_error("Could not set SO_REUSEADDR option on server socket"));
    }
    if (fcntl(this->_socketFd, F_SETFL, O_NONBLOCK) == -1)
    {
        throw(std::runtime_error("Could not set O_NONBLOCK option on server socket"));
    }
    if (bind(this->_socketFd, (struct sockaddr *) &address, sizeof(address)) == -1)
    {
        throw(std::runtime_error("Could not bind socket address to server socket"));
    }
    if (listen(this->_socketFd, SOMAXCONN) == -1)
    {
        throw(std::runtime_error("Could not set server socket to passive"));
    }

    newPoll.fd = this->_socketFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    this->_fds.push_back(newPoll);
}

void Server::serverInit(int port, std::string password)
{
    std::cout << "Initializing server ..." << std::endl;
    this->_port = port;
    this->_password = password;
    this->createSocket();
    std::cout << "Server initialized successfully ! <PORT " << this->_port << "> <PASSWORD " << this->_password << ">" << std::endl;
    this->_bot = new Bot();
    std::cout << "Waiting for client connection ..." << std::endl;

	while (Server::_signal == false)
	{
		if (poll(&this->_fds[0], this->_fds.size(), -1) == -1 && Server::_signal == false)
		{
			throw(std::runtime_error("poll function failed"));
		}
		for(size_t i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].revents & POLLIN)
			{
				if (this->_fds[i].fd == this->_socketFd)
				{
                    try
                    {
					    this->acceptClient();
                    }
                    catch (const std::runtime_error &e)
                    {
                        std::cout << "Error: " << e.what() << std::endl;
                    }
				}
				else if (this->getDccProxyByFd(this->_fds[i].fd) != NULL)
				{
					this->handleDccEvent(this->_fds[i].fd, this->_fds[i].revents);
				}
				else
				{
					this->newClientData(this->_fds[i].fd);
				}
			}
		}
	}
}

static std::vector<std::string> ft_split_irc(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) {
        if (!token.empty() && token[0] == ':') {
            std::string trailing = token.substr(1);
            std::string rest;
            std::getline(iss, rest);
            trailing += rest;
            tokens.push_back(trailing);
            break;
        }
        tokens.push_back(token);
    }

    return tokens;
}

static bool isUpperCmd(const std::string& line) {
    std::vector<std::string> tokens = ft_split_irc(line);
    if (tokens.empty())
        return false;
    const std::string& cmd = tokens[0];
    for (size_t i = 0; i < cmd.size(); i++) {
        if (!std::isupper(static_cast<unsigned char>(cmd[i])))
            return false;
    }
    return true;
}

static Command create_cmd(std::string line, Client *target, Server *server)
{
    std::vector<std::string> split_line = ft_split_irc(line);
    Command cmd(split_line[0], split_line, target, server);
    return cmd;
}

void Server::newClientData(int fd)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    Client *client = this->getClient(fd);
    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes > 0)
    {
        client->appendTempBuffer(std::string(buffer, bytes));

        while (true)
        {
            std::string pending = client->getTempBuffer();
            size_t pos = pending.find('\n');
            if (pos == std::string::npos)
                break;

            std::string line = pending.substr(0, pos);
            pending.erase(0, pos + 1);
            client->clearTempBuffer();
            client->appendTempBuffer(pending);

            if (!line.empty() && line[line.size()-1] == '\r')
                line.erase(line.size()-1);
            if (line.empty())
                continue;

            std::cout << "received line: " << line << std::endl;

            if (isUpperCmd(line))
            {
                std::vector<std::string> tokens = ft_split_irc(line);
                std::string cmdName = !tokens.empty() ? tokens[0] : "";
                std::string nick = client->getNickname().empty() ? "*" : client->getNickname();

                try
                {
                    Command cmd = create_cmd(line, client, this);
                    cmd.execCmd();
                }
                catch (const Command::UnknownCmd &e)
                {
                    std::string message = ":localhost 421 " + nick + " " + cmdName + " :Unknown command\r\n";
                    send(fd, message.c_str(), message.length(), 0);
                }
            }
        }
    }
    else
    {
        std::cout << "Client <" << client->getFd() << "> disconnected !" << std::endl;
        this->clearClient(fd);
        close(fd);
    }
}

Client *Server::getClient(int fd)
{
    for (std::list<Client>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); ++it)
    {
        if (it->getFd() == fd)
            return (&(*it));
    }
    return (NULL);
}

void Server::acceptClient()
{
    Client client;
    struct sockaddr_in clientAddress;
    struct pollfd newPoll;
    socklen_t size = sizeof(clientAddress);

    int requestFd = accept(this->_socketFd, (struct sockaddr *) &(clientAddress), &size);
    if (requestFd == -1)
    {
        std::cerr << "Failed to accept new client" << std::endl;
        return ;
    }
    if (fcntl(requestFd, F_SETFL, O_NONBLOCK) == -1)
    {
        throw(std::runtime_error("Could not set O_NONBLOCK option on server acceptation request"));
    }
    newPoll.fd = requestFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    client.setFd(requestFd);
    client.setIpAddress(inet_ntoa(clientAddress.sin_addr));
    client.setHostname(inet_ntoa(clientAddress.sin_addr));
    this->_clientList.push_back(client);
    this->_fds.push_back(newPoll);
    std::cout << "Client " << requestFd << " connected !" << std::endl;
}

std::vector<Channel> *Server::getChannels(void)
{
    return (&this->_channelList);
}

Bot *Server::getBot(void)
{
    return (this->_bot);
}

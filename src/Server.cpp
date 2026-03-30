#include "../includes/Server.hpp"
#include "../includes/Command.hpp"

bool Server::_signal = false;

Server::Server(void) 
{
    this->_socketFd = -1;
}

Server::~Server()
{}

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

std::vector<Client> *Server::getList(void)
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
	for (size_t i = 0; i < this->_clientList.size(); i++)
    {
		if (this->_clientList[i].getFd() == clientFd)
		{
            this->_clientList.erase(this->_clientList.begin() + i); 
            break;
        }
	}

}

void Server::closeFds(void)
{
    for (size_t i =0 ; i < this->_clientList.size(); i++)
    {
        std::cout << this->_clientList[i].getFd() << " disconnected" << std::endl;
        close(this->_clientList[i].getFd());
    }
    if (this->_socketFd != -1)
    {
        std::cout << "Server " << this->_socketFd << " disconnected" << std::endl;
        close(this->_socketFd);
    }
}

void Server::handleSignal(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
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
            if (!rest.empty() && rest[0] == ' ')
                rest.erase(0, 1);
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

            std::cout << "data received line: " << line << std::endl;

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
    for (size_t i = 0 ; i < this->_clientList.size(); i++)
    {
        if (this->_clientList[i].getFd() == fd)
            return (&this->_clientList[i]);
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

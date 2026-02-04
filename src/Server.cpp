#include "../includes/Server.hpp"

bool Server::_signal = false;

Server::Server(void) 
{
    this->_socketFd = -1;
    std::cout << "Default server constructor called" << std::endl;
}

Server::~Server()
{
    std::cout << "Server destructor called" << std::endl;
}

Server::Server(Server const &copy)
{
    *this = copy;
    std::cout << "Server copy constructor called" << std::endl;
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
		throw(std::runtime_error("faild to create socket"));
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

void Server::serverInit(void)
{
    std::cout << "Initializing server ..." << std::endl;
    this->_port = SERV_PORT;
    this->createSocket();
    std::cout << "Server initialized successfully !" << std::endl;
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
					this->acceptClient();
				}
				else
				{
					this->newClientData(this->_fds[i].fd);
				}
			}
		}
	}
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
    this->_clientList.push_back(client);
    this->_fds.push_back(newPoll);
    std::cout << "Client " << requestFd << " connected !" << std::endl;
}

void Server::newClientData(int fd)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    size_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        std::cout << "Client <" << fd << ">: " << buffer << std::endl;
    }
    else
    {
        std::cout << " Client <" << fd << "> disconnected !" << std::endl;
        this->clearClient(fd);
        close(fd);
    }
}

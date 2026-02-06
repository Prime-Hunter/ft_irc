#include "../includes/Server.hpp"

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
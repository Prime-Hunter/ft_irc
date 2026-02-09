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
    send(requestFd, "Welcome to IRC. Please enter server password: ", sizeof("Welcome to IRC. Please enter server password: ") - 1, 0);
    newPoll.fd = requestFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    client.setFd(requestFd);
    client.setIpAddress(inet_ntoa(clientAddress.sin_addr));
    this->_clientList.push_back(client);
    this->_fds.push_back(newPoll);
    std::cout << "Client " << requestFd << " connected !" << std::endl;
}

void Server::authClient(Client *client, char *buffer)
{
    if (!(this->_password.compare(buffer) + 1))
    {
        client->logIn();
        send(client->getFd(), "Correct password. you are now logged in !\n", sizeof("Correct password. you are now logged in !\n"), 0);
        std::cout << "Client <" << client->getFd() << "> logged in !" << std::endl;
    }
    else
    {
        send(client->getFd(), "Incorrect password. Please try again: ", sizeof("Incorrect password. Please try again: "), 0);
    }
}

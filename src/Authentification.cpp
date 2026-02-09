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
        send(client->getFd(), "Correct password. Define your username: ", sizeof("Correct password. Define your username: "), 0);
    }
    else
    {
        send(client->getFd(), "Incorrect password. Please try again: ", sizeof("Incorrect password. Please try again: "), 0);
    }
}

void Server::setClientUsername(Client *client, char *buffer)
{
    int i = 0;
    buffer[strcspn(buffer, "\n")] = 0;
    while (buffer[i] != '\0')
    {
        if (32 > buffer[i] || buffer[i] > 126)
        {
            send(client->getFd(), "Username must be printable. Try again: ", sizeof("Username must be printable. Try again: "), 0);
        }
    }
    if (i == 0)
    {
        send(client->getFd(), "Username cannot be empty. Try again: ", sizeof("Username cannot be empty. Try again: "), 0);
    }
    client->setUsername(buffer);
    send(client->getFd(), "Username assigned succesfully. Define your nickname: ", sizeof("Username assigned succesfully. Define your nickname: "), 0);
}

void Server::setClientNickname(Client *client, char *buffer)
{
    int i = 0;
    buffer[strcspn(buffer, "\n")] = 0;
    while (buffer[i] != '\0')
    {
        if (32 > buffer[i] || buffer[i] > 126)
        {
            send(client->getFd(), "Nickname must be printable. Try again: ", sizeof("Nickname must be printable. Try again: "), 0);
        }
        i++;
    }
    if (i == 0)
    {
        send(client->getFd(), "Nickname cannot be empty. Try again: ", sizeof("Nickname cannot be empty. Try again: "), 0);
    }
    client->setNickname(buffer);
    send(client->getFd(), "Nickame assigned succesfully. You are now logged in !", sizeof("Nickame assigned succesfully. You are now logged in !"), 0);
    std::cout << "Client <" << client->getFd() << "> logged in as " << client->getNickname() << " (" << client->getUsername() << ") !" << std::endl;
}
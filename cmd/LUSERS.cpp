#include "../includes/Command.hpp"

void Command::lusers(void)
{
    std::vector<Client> *clientList = _serv->getList();
    int totalUsers = clientList->size();
    
    std::string reply = Reply::lusersreply(_target->getNickname(), totalUsers);
    send(_target->getFd(), reply.c_str(), reply.length(), 0);
}

#include "../includes/Command.hpp"

void Command::names(void)
{
    if (_args.empty() || _args[1].empty()) {
        std::vector<Channel> *channels = _serv->getChannels();
        if (channels->empty()) {
            std::string endofnames = Reply::endofnames(_target->getPrefix(), "*");
            send(_target->getFd(), endofnames.c_str(), endofnames.length(), 0);
            return;
        }
        
        for (std::vector<Channel>::iterator it = channels->begin(); it != channels->end(); ++it) {
            std::string users = it->getUsers();
            std::string namreply = Reply::namreply(_target->getPrefix(), it->getName(), users);
            send(_target->getFd(), namreply.c_str(), namreply.length(), 0);
        }
        
        std::string endofnames = Reply::endofnames(_target->getPrefix(), "*");
        send(_target->getFd(), endofnames.c_str(), endofnames.length(), 0);
        return;
    }
    
    std::string channelName = _args[1];
    Channel *chan = getChannel(channelName);
    
    if (!chan) {
        std::string reply = Reply::nosuchchannel(_target->getNickname(), channelName);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    
    std::string users = chan->getUsers();
    std::string namreply = Reply::namreply(_target->getPrefix(), channelName, users);
    send(_target->getFd(), namreply.c_str(), namreply.length(), 0);
    
    std::string endofnames = Reply::endofnames(_target->getPrefix(), channelName);
    send(_target->getFd(), endofnames.c_str(), endofnames.length(), 0);
}

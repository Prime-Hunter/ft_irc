#include "../includes/Command.hpp"

void Command::quit(void)
{
    std::string quitMessage = "Client Quit";
    if (_args.size() > 1 && !_args[1].empty())
        quitMessage = _args[1];
    std::string fullQuit = Reply::quit(_target->getPrefix(), quitMessage);
    std::vector<Channel> *channels = _serv->getChannels();
    for (std::vector<Channel>::iterator it = channels->begin(); it != channels->end(); ++it)
    {
        if (it->isMember(_target))
        {
            it->broadcast(fullQuit, _target);
            it->removeMember(_target);
        }
    }
    send(_target->getFd(), fullQuit.c_str(), fullQuit.length(), 0);
}

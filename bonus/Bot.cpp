#include "../includes/Bot.hpp"
#include "../includes/Replies.hpp"
#include <sstream>

Bot::Bot()
{
    std::set<std::string> _words;
    std::set<std::string>::iterator it;
    _words.insert("42");
    _words.insert("Niel");
    _words.insert("ft_irc");
    _wordList = _words;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "IRC Bot online and listening" << std::endl;
    std::cout << "Forbidden words:\n";
    for (it = _wordList.begin(); it != _wordList.end(); ++it)
        std::cout << "\t- " << *it << std::endl; 
    std::cout << "----------------------------------" << std::endl;
}

Bot::Bot(std::set<std::string> wordList): _wordList(wordList)
{
    std::set<std::string>::iterator it;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "IRC Bot online and listening" << std::endl;
    std::cout << "Forbidden words:\n";
    for (it = _wordList.begin(); it != _wordList.end(); ++it)
        std::cout << "\t- " << *it << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

Bot::~Bot() 
{
    _wordList.clear();
    _warnList.clear();
}

Bot::Bot(Bot const &copy)
{
    *this = copy;
}

Bot &Bot::operator=(Bot const &src)
{
	if (this == &src)
    {
		return (*this);
    }
	this->_warnList = src._warnList;
	this->_wordList = src._wordList;
	return (*this);
}

void Bot::checkMsg(std::string msg, Channel *channel, Client *client)
{
    std::istringstream iss(msg);
    std::string word;
    while (iss >> word) {
        if (_wordList.find(word) != _wordList.end()) {
            warnClient(client, channel);
            return;
        }
    }
}

void Bot::warnClient(Client *client, Channel *channel)
{
    if (_warnList.find(client) == _warnList.end()) {
        _warnList[client] = 1;
    } else {
        _warnList[client]++;
    }
    if (_warnList[client] >= 3) {
        kickClient(client, channel);
    }
    else
    {
        std::string message = "Warning: " + client->getNickname() + " used a forbidden word.";
        channel->broadcast(Reply::privmsg("Bot!bot@localhost", channel->getName(), message), NULL);
    }
}

void Bot::kickClient(Client *client, Channel *channel)
{
    std::string reason = "Used forbidden words too many times";
    channel->broadcast(Reply::kick("Bot!bot@localhost", channel->getName(), client->getNickname(), reason), NULL);
    channel->removeMember(client);
    client->setChannel(NULL);
}
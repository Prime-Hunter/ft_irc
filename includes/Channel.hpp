#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <set>

class Client;

class Channel 
{
    private:
        std::string _name;
        std::string _topic;
        std::vector<Client *> _connectedClients;
        std::vector<Client *> _ops;
        std::string _key;
        int _inviteOnly;
        int _secured;
        int _userLimit;
        int _connectedCount;

    public:
        std::set<Client *> _list;
        Channel();
        Channel(const std::string &name);
        ~Channel();

        const std::string getName() const;
        const std::string getTopic() const;
        const std::string getKey() const;
        int getClientCount() const;
        int isInviteOnly() const;
        int isRestricted() const;
        int isOperator(Client* client) const;
        void setInviteOnly(int value);
        void setRestricted(int value);
        void setKey(const std::string &key);
        std::string const getKey();
        void setUserLimit(int limit);
        int getUserLimit() const;
        
        void addMember(Client* client);
        void addOperator(Client* client);
        void removeOperator(Client* client);
        void removeMember(Client* client);
        int isMember(Client* client) const;
        void setTopic(const std::string newTopic);
        void broadcast(std::string mess, Client *author);
        void sendMessage(std::string mess, Client *author);
        int isClientInvited(Client *target);
        void addInvite(Client *target);
        std::string getUsers(void);
};

#endif
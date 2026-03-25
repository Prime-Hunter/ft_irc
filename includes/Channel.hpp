#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

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
        int _connectedCount;

    public:
        Channel();
        Channel(const std::string &name);
        ~Channel();

        const std::string getName() const;
        const std::string getTopic() const;
        const std::string getKey() const;
        int getClientCount() const;
        int isInviteOnly() const;
        int isRestricted() const;
        
        void addMember(Client* client);
        void addOperator(Client* client);
        void removeMember(Client* client);
        int isMember(Client* client) const;
        void setTopic(const std::string newTopic);
};

#endif
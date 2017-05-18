//
// Created by dawid on 18.05.17.
//

#include "GroupServer.h"
#include <Ice/Ice.h>

void
Chat::GroupServerI::join(const ::Chat::UserPrx &who,
                         const Ice::Current &current) {
    if (find(users.begin(), users.end(), who) != users.end())
        throw UserAlreadyRegistered();

    users.push_back(who);
}

void
Chat::GroupServerI::Leave(const ::Chat::UserPrx &who,
                          const Ice::Current &current) {
    Users::iterator it = find(users.begin(), users.end(), who);
    if (it == users.end())
        throw UserDoesNotExist();

    users.erase(it);
}

void
Chat::GroupServerI::SendMessage(const ::std::string &message,
                                const ::Chat::UserPrx &sender,
                                const Ice::Current &current) {
    Users::iterator it;
    for (it = users.begin(); it != users.end(); ++it)
        if ((*it)->getName() == sender->getName())
            break;

    if (it == users.end())
        throw UserDoesNotExist();

    GroupServerPrx groupServerPrx = GroupServerPrx::uncheckedCast(current.adapter->createProxy(current.id));
    for (it = users.begin(); it != users.end(); ++it)
        (*it)->receiveText(message, sender, groupServerPrx);
}

::Chat::Users
Chat::GroupServerI::UserList(const Ice::Current &current) {
    return users;
}

::std::string
Chat::GroupServerI::Name(const Ice::Current &current) {
    return name;
}

Chat::GroupServerI::GroupServerI(const ::std::string &name) {
    this->name = name;
}
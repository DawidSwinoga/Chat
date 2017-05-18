//
// Created by dawid on 18.05.17.
//

#include "GroupServerManager.h"
#include "GroupServer.h"
#include "Ice/Ice.h"

::Chat::GroupServerPrx
Chat::GroupServerManagerI::CreateGroup(const ::std::string &name,
                                       const Ice::Current &current) {
    Ice::Identity id;
    id.name = name;

    if (current.adapter->find(id) != NULL)
        throw NameAlreadyExists();

    GroupServerPrx groupServerPrx = GroupServerPrx::uncheckedCast(current.adapter->add(new GroupServerI(name), id));
    groups.push_back(groupServerPrx);
    return groupServerPrx;
}

::Chat::Groups
Chat::GroupServerManagerI::ListGroups(const Ice::Current &current) {
    return groups;
}

void
Chat::GroupServerManagerI::DeleteGroup(const ::std::string &name,
                                       const Ice::Current &current) {
    Ice::Identity id;
    id.name = name;

    try {
        current.adapter->remove(id);
    } catch (Ice::NotRegisteredException &e) {
        throw NameDoesNotExist();
    }

    for (Groups::iterator it = groups.begin(); it != groups.end(); ++it) {
        if ((*it)->ice_getIdentity() == id) {
            groups.erase(it);
            break;
        }
    }
}

::Chat::GroupServerPrx
Chat::GroupServerManagerI::getGroupServerByName(const ::std::string &name,
                                                const Ice::Current &current) {
    Ice::Identity id;
    id.name = name;

    for (Groups::iterator it = groups.begin(); it != groups.end(); ++it) {
        if ((*it)->ice_getIdentity() == id)
            return *it;
    }

    return NULL;
}
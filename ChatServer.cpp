//
// Created by dawid on 18.05.17.
//

#include "ChatServer.h"

void
Chat::ChatServerI::LogIn(const ::Chat::UserPrx &callback,
                         const Ice::Current &current) {
    for (Users::iterator it = users.begin(); it != users.end(); ++it)
        if ((*it)->getName() == callback->getName())
            throw NameAlreadyExists();
    users.push_back(callback);
}

::Chat::UserPrx
Chat::ChatServerI::getUserByName(const ::std::string &name,
                                 const Ice::Current &current) {
    for (Users::iterator it = users.begin(); it != users.end(); ++it)
        if ((*it)->getName() == name)
            return *it;
    return NULL;
}

::Chat::Groups
Chat::ChatServerI::GroupList(const Ice::Current &current) {
    return groups;
}

::Chat::GroupServerPrx
Chat::ChatServerI::getGroupServerByName(const ::std::string &name,
                                        const Ice::Current &current) {
    for (Groups::iterator it = groups.begin(); it != groups.end(); ++it)
        if ((*it)->Name() == name)
            return *it;
    throw NameDoesNotExist();
}

void
Chat::ChatServerI::CreateGroup(const ::std::string &name,
                               const Ice::Current &current) {
    for (Groups::iterator it = groups.begin(); it != groups.end(); ++it)
        if ((*it)->Name() == name) {
            cout << "Group: " << name + " already exist" << endl;
            throw NameAlreadyExists();
        }
    cout << "New group: " + name << endl;
    GroupServerManagerPrx serverManagerPrx = serverManagers.at(groups.size() % serverManagers.size());
    groups.push_back(serverManagerPrx->CreateGroup(name));
}

void
Chat::ChatServerI::DeleteGroup(const ::std::string &name,
                               const Ice::Current &current) {
    Groups::iterator it;
    for (it = groups.begin(); it != groups.end(); ++it)
        if ((*it)->Name() == name)
            break;

    if (it == groups.end()) {
        cout << "Group: " + name + "does not exist" << endl;
        throw NameDoesNotExist();
    }

    GroupServerPrx groupServerPrx = *it;

    vector<IceInternal::ProxyHandle<IceProxy::Chat::GroupServerManager> >::iterator managerIt;
    for (managerIt = serverManagers.begin(); managerIt != serverManagers.end(); ++managerIt) {
        Groups groups = (*managerIt)->ListGroups();
        if (find(groups.begin(), groups.end(), groupServerPrx) != groups.end()) {
            (*managerIt)->DeleteGroup(groupServerPrx->Name());
            this->groups.erase(it);
            cout << "Group: " + name + " deleted" << endl;
            return;
        }
    }

    cout << "Group: " + name + "does not exist" << endl;
    throw NameDoesNotExist();
}

void
Chat::ChatServerI::registerServer(const ::Chat::GroupServerManagerPrx &serverManager,
                                  const Ice::Current &current) {
    if (find(serverManagers.begin(), serverManagers.end(), serverManager) != serverManagers.end()) {
        cout << "Group server manager already exist" << endl;
        throw ServerAlreadyRegistered();
    }

    serverManagers.push_back(serverManager);
    cout << "Add new group server manager" << endl;
}

void
Chat::ChatServerI::unregisterServer(const ::Chat::GroupServerManagerPrx &serverManager,
                                    const Ice::Current &current) {
    vector<IceInternal::ProxyHandle<IceProxy::Chat::GroupServerManager> >::iterator it =
            find(serverManagers.begin(), serverManagers.end(), serverManager);
    if (it == serverManagers.end()) {
        cout << "Server group manager does not exist" << endl;
        throw ServerDoesNotExist();
    }

    serverManagers.erase(it);
    cout << "Server group manager removed" << endl;
}
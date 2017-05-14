
#include <chatI.h>
#include <Ice/Ice.h>
#include <Ice/Identity.h>
using namespace std;

void
Chat::ChatServerI::LogIn(const ::Chat::UserPrx& callback,
                         const Ice::Current& current)
{
    if (find(users.begin(), users.end(), callback) != users.end())
        throw NameAlreadyExists();
    users.push_back(callback);
}

::Chat::UserPrx
Chat::ChatServerI::getUserByName(const ::std::string& name,
                                 const Ice::Current& current)
{
    for (Users::iterator it = users.begin(); it != users.end(); ++it)
        if ((*it)->getName() == name)
            return *it;
    return NULL;
}

::Chat::Groups
Chat::ChatServerI::GroupList(const Ice::Current& current)
{
    return groups;
}

::Chat::GroupServerPrx
Chat::ChatServerI::getGroupServerByName(const ::std::string& name,
                                        const Ice::Current& current)
{
    for (Groups::iterator it = groups.begin(); it != groups.end(); ++it)
        if ((*it)->Name() == name)
            return *it;
    return NULL;
}

void
Chat::ChatServerI::CreateGroup(const ::std::string& name,
                               const Ice::Current& current)
{
    for (Groups::iterator it = groups.begin(); it != groups.end(); ++it)
        if ((*it)->Name() == name)
            throw NameAlreadyExists();

    GroupServerManagerPrx serverManagerPrx = serverManagers.at(groups.size() % serverManagers.size());
    groups.push_back(serverManagerPrx->CreateGroup(name));
}

void
Chat::ChatServerI::DeleteGroup(const ::std::string& name,
                               const Ice::Current& current)
{
    Groups::iterator it;
    for (it = groups.begin(); it != groups.end(); ++it)
        if ((*it)->Name() == name)
            break;

    if (it == groups.end())
        throw NameDoesNotExist();

    GroupServerPrx groupServerPrx = *it;

    vector<IceInternal::ProxyHandle<IceProxy::Chat::GroupServerManager>>::iterator managerIt;
    for (managerIt = serverManagers.begin(); managerIt != serverManagers.end(); ++managerIt ) {
        Groups groups = (*managerIt)->ListGroups();
        if(find(groups.begin(), groups.end(), groupServerPrx) != groups.end()) {
            (*managerIt)->DeleteGroup(groupServerPrx->Name());
            this->groups.erase(it);
            return;
        }
    }

    throw NameDoesNotExist();
}

void
Chat::ChatServerI::registerServer(const ::Chat::GroupServerManagerPrx& serverManager,
                                  const Ice::Current& current)
{
    if (find(serverManagers.begin(), serverManagers.end(), serverManager) != serverManagers.end())
        throw ServerAlreadyRegistered();

    serverManagers.push_back(serverManager);
}

void
Chat::ChatServerI::unregisterServer(const ::Chat::GroupServerManagerPrx& serverManager,
                                    const Ice::Current& current)
{
    vector<IceInternal::ProxyHandle<IceProxy::Chat::GroupServerManager>>::iterator it =
            find(serverManagers.begin(), serverManagers.end(), serverManager);
    if (it == serverManagers.end())
        throw ServerDoesNotExist();

    serverManagers.erase(it);
}

void
Chat::GroupServerI::join(const ::Chat::UserPrx& who,
                         const Ice::Current& current)
{
    if (find(users.begin(), users.end(), who) != users.end())
        throw UserAlreadyRegistered();

    users.push_back(who);
}

void
Chat::GroupServerI::Leave(const ::Chat::UserPrx& who,
                          const Ice::Current& current)
{
    Users::iterator it = find(users.begin(), users.end(), who);
    if (it == users.end())
        throw UserDoesNotExist();

    users.erase(it);
}

void
Chat::GroupServerI::SendMessage(const ::std::string& message,
                                const ::Chat::UserPrx& sender,
                                const Ice::Current& current)
{
    if (find(users.begin(), users.end(), sender) == users.end())
        throw UserDoesNotExist();


    GroupServerPrx groupServerPrx = GroupServerPrx::uncheckedCast(current.adapter->createProxy(current.id));
    for(Users::iterator it = users.begin(); it != users.end(); ++it)
        (*it)->receiveText(message, sender, groupServerPrx);
}

::Chat::Users
Chat::GroupServerI::UserList(const Ice::Current& current)
{
    return users;
}

::std::string
Chat::GroupServerI::Name(const Ice::Current& current)
{
    return name;
}

Chat::GroupServerI::GroupServerI(const ::std::string & name) {
    this->name = name;
}

::Chat::GroupServerPrx
Chat::GroupServerManagerI::CreateGroup(const ::std::string& name,
                                       const Ice::Current& current)
{
    Ice::Identity id;
    id.name = name;

    if (current.adapter->find(id) != NULL)
        throw NameAlreadyExists();

    GroupServerPrx groupServerPrx = GroupServerPrx::uncheckedCast(current.adapter->add(new GroupServerI(name), id));
    groups.push_back(groupServerPrx);
    return groupServerPrx;
}

::Chat::Groups
Chat::GroupServerManagerI::ListGroups(const Ice::Current& current)
{
    return groups;
}

void
Chat::GroupServerManagerI::DeleteGroup(const ::std::string& name,
                                       const Ice::Current& current)
{
    Ice::Identity id;
    id.name = name;

    try {
        current.adapter->remove(id);
    } catch (Ice::NotRegisteredException& e) {
        throw NameDoesNotExist();
    }

    for(Groups::iterator it = groups.begin(); it != groups.end(); ++it) {
        if ((*it)->ice_getIdentity() == id) {
            groups.erase(it);
            break;
        }
    }
}

::Chat::GroupServerPrx
Chat::GroupServerManagerI::getGroupServerByName(const ::std::string& name,
                                                const Ice::Current& current)
{
    Ice::Identity id;
    id.name = name;

    for(Groups::iterator it = groups.begin(); it != groups.end(); ++it) {
        if ((*it)->ice_getIdentity() == id)
            return *it;
    }

    return NULL;
}

::std::string
Chat::UserI::getName(const Ice::Current& current)
{
    return username;
}

void
Chat::UserI::receiveText(const ::std::string& msg,
                         const ::Chat::UserPrx& sender,
                         const ::Chat::GroupServerPrx& gServer,
                         const Ice::Current& current)
{
    string groupName = gServer->Name();
    string senderName = sender->getName();
    cout << "Group " << groupName << ": " << senderName << ": " << msg;
}

void
Chat::UserI::receivePrivateText(const ::std::string& msg,
                                const ::Chat::UserPrx& sender,
                                const Ice::Current& current)
{
    cout << "Priv from: " + sender->getName() + "message: " + msg;
}

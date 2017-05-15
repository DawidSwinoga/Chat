#ifndef __chatI_h__
#define __chatI_h__

#include <chat.h>
#include <string>
#include <vector>

using namespace std;

namespace Chat
{

class ChatServerI : virtual public ChatServer
{
public:

    virtual void LogIn(const ::Chat::UserPrx&,
                       const Ice::Current&);

    virtual ::Chat::UserPrx getUserByName(const ::std::string&,
                                          const Ice::Current&);

    virtual ::Chat::Groups GroupList(const Ice::Current&);

    virtual ::Chat::GroupServerPrx getGroupServerByName(const ::std::string&,
                                                        const Ice::Current&);

    virtual void CreateGroup(const ::std::string&,
                             const Ice::Current&);

    virtual void DeleteGroup(const ::std::string&,
                             const Ice::Current&);

    virtual void registerServer(const ::Chat::GroupServerManagerPrx&,
                                const Ice::Current&);

    virtual void unregisterServer(const ::Chat::GroupServerManagerPrx&,
                                  const Ice::Current&);
    Users users;
    Groups groups;
    vector<GroupServerManagerPrx> serverManagers;
};

class GroupServerI : virtual public GroupServer
{
public:
    GroupServerI(const ::std::string&);

    virtual void join(const ::Chat::UserPrx&,
                      const Ice::Current&);

    virtual void Leave(const ::Chat::UserPrx&,
                       const Ice::Current&);

    virtual void SendMessage(const ::std::string&,
                             const ::Chat::UserPrx&,
                             const Ice::Current&);

    virtual ::Chat::Users UserList(const Ice::Current&);

    virtual ::std::string Name(const Ice::Current&);

private:
    Users users;
    string name;
};

class GroupServerManagerI : virtual public GroupServerManager
{
public:

    virtual ::Chat::GroupServerPrx CreateGroup(const ::std::string&,
                                               const Ice::Current&);

    virtual ::Chat::Groups ListGroups(const Ice::Current&);

    virtual void DeleteGroup(const ::std::string&,
                             const Ice::Current&);

    virtual ::Chat::GroupServerPrx getGroupServerByName(const ::std::string&,
                                                        const Ice::Current&);

private:
    Groups groups;
};

class UserI : virtual public User
{
public:
    UserI(string name);

    virtual ::std::string getName(const Ice::Current&);

    virtual void receiveText(const ::std::string&,
                             const ::Chat::UserPrx&,
                             const ::Chat::GroupServerPrx&,
                             const Ice::Current&);

    virtual void receivePrivateText(const ::std::string&,
                                    const ::Chat::UserPrx&,
                                    const Ice::Current&);

private:
    string username;
};

}

#endif

//
// Created by dawid on 18.05.17.
//

#ifndef CHAT_CHATSERVERI_H
#define CHAT_CHATSERVERI_H

#include "chat.h"
#include <vector>

using namespace std;

namespace Chat {

    class ChatServerI : virtual public ChatServer {
    public:

        virtual void LogIn(const ::Chat::UserPrx &,
                           const Ice::Current &);

        virtual ::Chat::UserPrx getUserByName(const ::std::string &,
                                              const Ice::Current &);

        virtual ::Chat::Groups GroupList(const Ice::Current &);

        virtual ::Chat::GroupServerPrx getGroupServerByName(const ::std::string &,
                                                            const Ice::Current &);

        virtual void CreateGroup(const ::std::string &,
                                 const Ice::Current &);

        virtual void DeleteGroup(const ::std::string &,
                                 const Ice::Current &);

        virtual void registerServer(const ::Chat::GroupServerManagerPrx &,
                                    const Ice::Current &);

        virtual void unregisterServer(const ::Chat::GroupServerManagerPrx &,
                                      const Ice::Current &);

        Users users;
        Groups groups;
        vector<GroupServerManagerPrx> serverManagers;
    };
};

#endif //CHAT_CHATSERVERI_H

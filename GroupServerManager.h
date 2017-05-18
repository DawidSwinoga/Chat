//
// Created by dawid on 18.05.17.
//

#ifndef CHAT_GROUPSERVERMANAGERI_H
#define CHAT_GROUPSERVERMANAGERI_H

#include "chat.h"
#include <string>

using namespace std;

namespace Chat {

    class GroupServerManagerI : virtual public GroupServerManager {
    public:

        virtual ::Chat::GroupServerPrx CreateGroup(const ::std::string &,
                                                   const Ice::Current &);

        virtual ::Chat::Groups ListGroups(const Ice::Current &);

        virtual void DeleteGroup(const ::std::string &,
                                 const Ice::Current &);

        virtual ::Chat::GroupServerPrx getGroupServerByName(const ::std::string &,
                                                            const Ice::Current &);

    private:
        Groups groups;
    };
};

#endif //CHAT_GROUPSERVERMANAGERI_H

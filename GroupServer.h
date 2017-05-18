//
// Created by dawid on 18.05.17.
//

#ifndef CHAT_GROUPSERVERI_H
#define CHAT_GROUPSERVERI_H

#include <string>
#include "chat.h"

namespace Chat {

    class GroupServerI : virtual public GroupServer {
    public:
        GroupServerI(const ::std::string &);

        virtual void join(const ::Chat::UserPrx &,
                          const Ice::Current &);

        virtual void Leave(const ::Chat::UserPrx &,
                           const Ice::Current &);

        virtual void SendMessage(const ::std::string &,
                                 const ::Chat::UserPrx &,
                                 const Ice::Current &);

        virtual ::Chat::Users UserList(const Ice::Current &);

        virtual ::std::string Name(const Ice::Current &);

    private:
        Users users;
        std::string name;
    };
};
#endif //CHAT_GROUPSERVERI_H

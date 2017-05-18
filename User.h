//
// Created by dawid on 18.05.17.
//

#ifndef CHAT_USERI_H
#define CHAT_USERI_H

#include "chat.h"
#include <string>

using namespace std;

namespace Chat {

    class UserI : virtual public User {
    public:
        UserI(string name);

        virtual ::std::string getName(const Ice::Current &);

        virtual void receiveText(const ::std::string &,
                                 const ::Chat::UserPrx &,
                                 const ::Chat::GroupServerPrx &,
                                 const Ice::Current &);

        virtual void receivePrivateText(const ::std::string &,
                                        const ::Chat::UserPrx &,
                                        const Ice::Current &);

    private:
        string username;
    };
};

#endif //CHAT_USERI_H

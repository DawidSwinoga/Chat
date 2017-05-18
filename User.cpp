//
// Created by dawid on 18.05.17.
//

#include "User.h"

::std::string
Chat::UserI::getName(const Ice::Current &current) {
    return username;
}

void
Chat::UserI::receiveText(const ::std::string &msg,
                         const ::Chat::UserPrx &sender,
                         const ::Chat::GroupServerPrx &gServer,
                         const Ice::Current &current) {
    string senderName = sender->getName();
    string groupName = gServer->ice_getIdentity().name;
    cout << "Group " << groupName << ": " << senderName << ": " << msg << endl;
}

void
Chat::UserI::receivePrivateText(const ::std::string &msg,
                                const ::Chat::UserPrx &sender,
                                const Ice::Current &current) {
    cout << "Priv from: " + sender->getName() + "message: " + msg << endl;
}

Chat::UserI::UserI(string name) {
    this->username = name;
}

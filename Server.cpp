#include <iostream>
#include <Ice/Ice.h>
#include "ChatServer.h"

using namespace std;
using namespace Chat;

int main(int argc, char *argv[]) {
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter =
                ic->createObjectAdapterWithEndpoints("ServerChatAdapter", "default -p 10000");
        Ice::ObjectPtr object = new ChatServerI();
        adapter->add(object, ic->stringToIdentity("ServerChat"));
        adapter->activate();
        ic->waitForShutdown();
    } catch (const Ice::Exception &e) {
        cerr << e << endl;
        status = 1;
    } catch (const char *msg) {
        cerr << msg << endl;
        status = 1;
    }
    if (ic) {
        try {
            ic->destroy();
        } catch (const Ice::Exception &e) {
            cerr << e << endl;
            status = 1;
        }
    }
    return status;
}
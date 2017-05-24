#include <iostream>
#include <Ice/Ice.h>
#include "chat.h"
#include "GroupServerManager.h"

using namespace std;
using namespace Chat;


int main(int argc, char *argv[]) {
    int status = 0;
    Ice::CommunicatorPtr ic;
    string text;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter =
                ic->createObjectAdapterWithEndpoints("ServerManagerAdapter", "default -p 10004");
        GroupServerManagerPtr groupServerManagerPtr = new GroupServerManagerI();
        GroupServerManagerPrx groupServerManagerPrx =
                GroupServerManagerPrx::uncheckedCast(adapter->addWithUUID(groupServerManagerPtr));
        adapter->activate();
        Ice::ObjectPrx base = ic->stringToProxy("ServerChat:default -p 10000");
        ChatServerPrx chatServerPrx = ChatServerPrx::checkedCast(base);
        if (!chatServerPrx)
            throw "Invalid proxy";

        chatServerPrx->registerServer(groupServerManagerPrx);
        do {
            cin >> text;
        } while (text != "exit");
        chatServerPrx->unregisterServer(groupServerManagerPrx);
        ic->shutdown();
        ic->waitForShutdown();
    } catch (const Ice::Exception &ex) {
        cerr << ex << endl;
        status = 1;
    } catch (const char *msg) {
        cerr << msg << endl;
        status = 1;
    }
    if (ic)
        ic->destroy();
    return status;
}

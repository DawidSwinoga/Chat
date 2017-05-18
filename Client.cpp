#include <iostream>
#include <Ice/Ice.h>
#include "chat.h"
#include "User.h"

using namespace std;
using namespace Chat;

class ClientApp : virtual public Ice::Application {
    Ice::ObjectAdapterPtr adapter;
    UserPrx userPrx;
    ChatServerPrx chatServerPrx;
    GroupServerPrx groupServerPrx = NULL;

    string getName() {
        string name;
        cout << "name: ";
        cin >> name;
        return name;
    }


    void createGroup() {
        try {
            chatServerPrx->CreateGroup(getName());
        } catch (const NameAlreadyExists &ex) {
            cout << "Name already exist" << endl;
        }

    }

    void deleteGroup() {
        string name = getName();
        try {
            if (name == groupServerPrx->Name())
                chatServerPrx->DeleteGroup(name);
        } catch (const NameDoesNotExist &ex) {
            cout << "Name does not exist" << endl;
        }
    }

    void printGroupNames() {
        Groups groups = chatServerPrx->GroupList();
        for (Groups::iterator it = groups.begin(); it != groups.end(); ++it) {
            cout << (*it)->Name() << endl;
        }
    }

    void selectGroup() {
        try {
            groupServerPrx = chatServerPrx->getGroupServerByName(getName());
            groupServerPrx->join(userPrx);
        } catch (const UserAlreadyRegistered &ex) {
            cout << "Name does not exist" << endl;
            groupServerPrx = NULL;
        }

    }

    void sendGroupMessage() {
        cout << "Message: ";
        string message;
        cin >> message;

        try {
            groupServerPrx->SendMessage(message, userPrx);
        } catch (const UserDoesNotExist &ex) {
            cout << "You are not a registered user" << endl;
        } catch (const Ice::ObjectNotExistException &ex) {
            cout << "Group is not available" << endl;
            groupServerPrx = NULL;
        }
    }

    void sendPriv() {
        UserPrx user = chatServerPrx->getUserByName(getName());
        if (user == NULL) {
            cout << "User does not exist";
            return;
        }

        string message;
        cout << "message: ";
        cin >> message;
        user->receivePrivateText(message, userPrx);
    }

    virtual int run(int, char *[]) {
        initAdapter();
        initChatServerPrx();
        registerUser();
        int option = -1;

        while (true) {
            displayMenu();
            cout << "Option: ";
            cin >> option;

            switch (option) {
                case 1:
                    createGroup();
                    break;
                case 2:
                    deleteGroup();
                    break;
                case 3:
                    printGroupNames();
                    break;
                case 4:
                    if (groupServerPrx != NULL) {
                        groupServerPrx->Leave(userPrx);
                    }
                    printGroupNames();
                    selectGroup();
                    break;
                case 5:
                    sendPriv();
                    break;
                case 6:
                    if (groupServerPrx == NULL) {
                        cout << "First select a group" << endl;
                        break;
                    }
                    sendGroupMessage();
                    break;
                default:
                    break;
            }
        }
        return 0;
    }

    void initChatServerPrx() {
        try {
            Ice::ObjectPrx base = communicator()->stringToProxy("ServerChat:default -p 10000");
            chatServerPrx = ChatServerPrx::checkedCast(base);
        } catch (const Ice::Exception &ex) {
            cerr << ex << endl;
            cout << "fatal error";
            exit(1);
        }
    }

    void initAdapter() {
        adapter = communicator()->createObjectAdapterWithEndpoints("ClientAdapter", "default -p 10021");
    }

    void displayMenu() {
        cout << "1. Create group" << endl;
        cout << "2. Delete group" << endl;
        cout << "3. Group list" << endl;
        cout << "4. Select group" << endl;
        cout << "5. Send private message" << endl;
        cout << "6. Send group message" << endl;
    }

    void registerUser() {
        string username;
        UserPtr userPtr;

        while (1) {
            cout << "Username: ";
            cin >> username;

            userPtr = new UserI(username);
            userPrx = UserPrx::uncheckedCast(adapter->addWithUUID(userPtr));
            adapter->activate();

            try {
                chatServerPrx->LogIn(userPrx);
                return;
            } catch (const NameAlreadyExists &ex) {
                cout << "Name already exist" << endl;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    ClientApp app;
    return app.main(argc, argv);
}
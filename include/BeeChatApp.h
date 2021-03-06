// BeeChatApp.h

#ifndef _BEECHAT_APP_H
#define _BEECHAT_APP_H

#include "Server.h"
#include "Client.h"
#include "MessageFactory.h"
#include <string>

class BeeChatApp
{
private:
    
    std::string username; 
    
    //LachlansUIThing* ui
    class MessageFactory* messageFactory;
  
    // server object
    class Server* server;

    // client object
    class Client* client;
  


public: 

    BeeChatApp(std::string username, byte usernameColor, bool userWantsServer);
    ~BeeChatApp();
    int quit;
    int Run();

};

// _BEECHAT_APP_H
#endif

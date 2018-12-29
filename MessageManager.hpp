//
//  MessageManager.hpp
//  server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef MessageManager_hpp
#define MessageManager_hpp

#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include "Server.hpp"
#include "Client.hpp"
#include "GameManager.hpp"

using namespace std;

class MessageManager{
public:
    static void resolveMessage(string msg, int socket);
    static void sendMessage(string msg, int socket);
    static void sendToAll(string msg);
private:
    string msg;
};

#endif /* MessageManager_hpp */

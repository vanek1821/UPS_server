//
//  GameManager.hpp
//  server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>
#include <iostream>
#include <map>
#include <pthread.h>
#include "MessageManager.hpp"
#include "Client.hpp"
#include "Match.hpp"
#include "Color.hpp"
#include "Field.hpp"
#include "Game.hpp"

using namespace std;

class GameManager{
public:
    static void addToClientMap(Client* client);
    static bool connectClient(string name, int socket);
    static int createMatch(int clientID);
    static int joinMatch(string matchS, int clientID);
    static void initGame(int matchID);
    static string findMatches();
    static Client* findClient(int clientID);
    static Client* findClientByName(string name);
    static Match* findMatch(int matchID);
    static map<int, Client*> getClientMap() {return clientMap;};
    static bool replayMatch(int socketID);
    static void leaveMatch(int matchID);
    static void removeClientByID(int socketID);
    static void removeClientByName(string name);
    static bool clientExists(string name, int socketID);
    static void reconnectClient(int socketID);
    static void sendGame(int socketID);
    static void moveToDisconnected(int socketID);
private:
    static map<int, Client*> clientMap;
    static map<int, Match*> matchMap;
    static map<string, Client*> disconnectedMap;
};

#endif /* GameManager_hpp */

//
//  Client.hpp
//  server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "ClientState.hpp"
#include "Color.hpp"

using namespace std;

class Client{
public:
    Client(string name, int socketID);
    int getSocketID()   {return this->socketID;};
    void setSocketID(int socket) {this->socketID = socket;};
    string getName() {return this->name;};
    void setState(ClientState state) {this->state = state;};
    ClientState getState() {return this->state;};
    void setColor(COLOR color) {this->color = color;};
    COLOR getColor() {return this->color;};
    bool isTurn() {return this->turn;};
    void setTurn(bool turn) {this->turn = turn;};
    int getMatchID() {return this->matchID;};
    void setMatchID(int matchID) {this->matchID = matchID;};
    void setInGame(bool state) {this->inGame = state;};
    bool getInGame() {return this->inGame;};
    void setConnected(bool state) {this->connected = state;};
    bool isConnected() {return this->connected;};
    bool getExisting(){return this->existing;};
    void setExisting(bool existing){this->existing = existing;};
    void setResult(bool result){this->result = result;};
    bool getResult(){return this->result;};
    pthread_t thread;
    
private:
    int socketID;
    ClientState state;
    string name;
    bool inGame;
    bool turn;
    COLOR color;
    int matchID;
    bool connected;
    bool existing;
    bool result;
    
};

#endif /* Client_hpp */

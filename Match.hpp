//
//  Match.hpp
//  server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef Match_hpp
#define Match_hpp

#include <stdio.h>
#include <iostream>
#include "Client.hpp"
#include "Game.hpp"

using namespace std;

class Match{
public:
    Match(int matchID);
    void setClient1(Client* c) {this->c1 = c;};
    void setClient2(Client* c) {this->c2 = c;};
    Client* getClient1() {return this->c1;};
    Client* getClient2() {return this->c2;};
    void setGame(Game* game) {this->game = game;};
    Game* getGame() {return this->game;};
    bool isReady() {return this->ready;};
    void setReady(bool val) {this->ready = val;};
    int getID() {return this->matchID;};
private:
    bool ready;
    int matchID;
    Client* c1;
    Client* c2;
    Game* game;
};

#endif /* Match_hpp */

//
//  Game.hpp
//  server
//
//  Created by Jakub  Vaněk on 11/12/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include "Client.hpp"
#include "Field.hpp"
#include "Type.hpp"
#include "Color.hpp"

using namespace std;

class Game{
public:
    Game();
    int move(Client* client, int sRow, int sCol, int dRow, int dCol);
    bool checkMove(Client* client, int sRow, int sCol, int dRow, int dCol);
    Field* field[9][9];
    
private:
    
    void initGame();
    void initPieces();
    bool checkKingMove(int sRow, int sCol, int dRow, int dCol);
    bool checkQueenMove(int sRow, int sCol, int dRow, int dCol);
    bool checkKnightMove(int sRow, int sCol, int dRow, int dCol);
    bool checkBishopMove(int sRow, int sCol, int dRow, int dCol);
    bool checkRookMove(int sRow, int sCol, int dRow, int dCol);
    bool checkPawnMove(int sRow, int sCol, int dRow, int dCol);
    bool checkWhitePawnMove(int sRow, int sCol, int dRow, int dCol);
    bool checkBlackPawnMove(int sRow, int sCol, int dRow, int dCol);
};

#endif /* Game_hpp */

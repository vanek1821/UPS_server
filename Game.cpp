//
//  Game.cpp
//  server
//
//  Created by Jakub  Vaněk on 11/12/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#include "Game.hpp"

Game::Game(){
    initGame();
}

void Game::initGame(){
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++){
            field[i][j] = new Field();
            field[i][j]->setPiece(NULL);
        }
    }
    cout << "nainicializovano pole fieldů"<<endl;
    Game::initPieces();
}
void Game::initPieces(){
    //Kings
    field[1][5]->setPiece(new Piece(TYPE::KING, COLOR::WHITE));
    field[8][5]->setPiece(new Piece(TYPE::KING, COLOR::BLACK));
    cout << "Kingové inicializováni" << endl;
    //Queens
    field[1][4]->setPiece(new Piece(TYPE::QUEEN, COLOR::WHITE));
    field[8][4]->setPiece(new Piece(TYPE::QUEEN, COLOR::BLACK));
    cout << "Queeny inicializovány" << endl;
    //Bishops
    field[1][3]->setPiece(new Piece(TYPE::BISHOP, COLOR::WHITE));
    field[1][6]->setPiece(new Piece(TYPE::BISHOP, COLOR::WHITE));
    field[8][3]->setPiece(new Piece(TYPE::BISHOP, COLOR::BLACK));
    field[8][6]->setPiece(new Piece(TYPE::BISHOP, COLOR::BLACK));
    cout << "Bishopové inicializováni" << endl;
    //Kinghts
    field[1][2]->setPiece(new Piece(TYPE::KNIGHT, COLOR::WHITE));
    field[1][7]->setPiece(new Piece(TYPE::KNIGHT, COLOR::WHITE));
    field[8][2]->setPiece(new Piece(TYPE::KNIGHT, COLOR::BLACK));
    field[8][7]->setPiece(new Piece(TYPE::KNIGHT, COLOR::BLACK));
    cout << "Knightové inicializováni" << endl;
    //Rook
    field[1][1]->setPiece(new Piece(TYPE::ROOK, COLOR::WHITE));
    field[1][8]->setPiece(new Piece(TYPE::ROOK, COLOR::WHITE));
    field[8][1]->setPiece(new Piece(TYPE::ROOK, COLOR::BLACK));
    field[8][8]->setPiece(new Piece(TYPE::ROOK, COLOR::BLACK));
    //Pawn
    for(int i = 1; i<=8; i++){
        field[2][i]->setPiece(new Piece(TYPE::PAWN, COLOR::WHITE));
        field[7][i]->setPiece(new Piece(TYPE::PAWN, COLOR::BLACK));
    }
    cout << "Pawnové inicializováni" << endl;
}

int Game::move(Client* client, int sRow, int sCol, int dRow, int dCol){
    if(checkMove(client, sRow, sCol, dRow, dCol)){
        if(field[dRow][dCol]->getPiece() != NULL){
            if(field[dRow][dCol]->getPiece()->getType() == TYPE::KING) {
                return 2;
            }
        }
        field[dRow][dCol]->setPiece(field[sRow][sCol]->getPiece());
        field[sRow][sCol]->setPiece(NULL);
        return 1;
    }
    else return 0;
}
bool Game::checkMove(Client* client, int sRow, int sCol, int dRow, int dCol){
    if(field[sRow][sCol]->getPiece() == NULL){
        return false;
    }
    
    if(field[dRow][dCol]->getPiece() != NULL){
        if(field[dRow][dCol]->getPiece()->getColor() == client->getColor()){
            return false;
        }
    }
    
    
    if(field[sRow][sCol]->getPiece()->getColor() == client->getColor()){
        switch (field[sRow][sCol]->getPiece()->getType()) {
            case TYPE::KING:
                return Game::checkKingMove(sRow, sCol, dRow, dCol);
                break;
            case TYPE::QUEEN:
                return Game::checkQueenMove(sRow, sCol, dRow, dCol);
                break;
            case TYPE::KNIGHT:
                return Game::checkKnightMove(sRow, sCol, dRow, dCol);
                break;
            case TYPE::BISHOP:
                return Game::checkBishopMove(sRow, sCol, dRow, dCol);
                    break;
            case TYPE::ROOK:
                return Game::checkRookMove(sRow, sCol, dRow, dCol);
                break;
            case TYPE::PAWN:
                return Game::checkPawnMove(sRow, sCol, dRow, dCol);
                break;
        }
    }
    return false;
}
bool Game::checkKingMove(int sRow, int sCol, int dRow, int dCol){
    if((abs(sRow-dRow) <= 1) && (abs(sCol-dCol) <= 1)) return true;
    else return false;
}


//Queen Moves
bool Game::checkQueenMove(int sRow, int sCol, int dRow, int dCol){
    //StraightMove
    // Row move
    if (sCol == dCol) {
        int col = sCol;
        int rowDif = dRow - sRow;
        //iterace mezi pozicemi
        for (int i = 1; i <= abs(rowDif); i++) {
            int row;
            if (rowDif > 0) {
                row = sRow + i;
            } else {
                row = sRow - i;
            }
            // Occupied
            if (field[row][col]->getPiece() != NULL) {
                if(row!=dRow || col!=dCol){
                    return false;
                }
            }
        }
        return true;
        // Column move
    } else if (sRow == dRow) {
        int row = sRow;
        int colDif = dCol - sCol;
        //iterace mezi pozicemi
        for (int i = 1; i <= abs(colDif); i++) {
            int col;
            if (colDif > 0) {
                col = sCol + i;
            } else {
                col = sCol - i;
            }
            
            // Occupied
            if (field[row][col]->getPiece() != NULL) {
                if(row!=dRow || col!=dCol){
                    return false;
                }
            }
        }
        return true;
    }
    else {
        int rowDif = dRow - sRow;
        int colDif = dCol - sCol;
        if (abs(colDif) == abs(rowDif)) {
            // Iterates over fields between old and last postition
            for (int i = 1; i <= abs(rowDif); i++) {
                int row, col;
                if (rowDif > 0) {
                    row = sRow + i;
                } else {
                    row = sRow - i;
                }
                
                if (colDif > 0) {
                    col = sCol + i;
                } else {
                    col = sCol - i;
                }
                
                // Checks if fields are occupied
                if (field[row][col]->getPiece() != NULL) {
                    if(row!=dRow || col!=dCol){
                        return false;
                    }
                }
            }
            return true;
        } else {
            return false;
        }
    }
}


//Knight Moves
bool Game::checkKnightMove(int sRow, int sCol, int dRow, int dCol){
    int rowDif = dRow - sRow;
    int colDif = dCol - sCol;
    if ((abs(rowDif) == 2 && abs(colDif) == 1) || (abs(rowDif) == 1 && abs(colDif) == 2)) {
        return true;
    } else {
        return false;
    }
}


//Bishop Moves
bool Game::checkBishopMove(int sRow, int sCol, int dRow, int dCol){
    int rowDif = dRow - sRow;
    int colDif = dCol - sCol;
    if (abs(colDif) == abs(rowDif)) {
        // Iterates over fields between old and last postition
        for (int i = 1; i <= abs(rowDif); i++) {
            int row, col;
            if (rowDif > 0) {
                row = sRow + i;
            } else {
                row = sRow - i;
            }
            
            if (colDif > 0) {
                col = sCol + i;
            } else {
                col = sCol - i;
            }
            
            // Checks if fields are occupied
            if (field[row][col]->getPiece() != NULL) {
                if(row!=dRow || col!=dCol){
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

//Rook Moves
bool Game::checkRookMove(int sRow, int sCol, int dRow, int dCol){
    // Row move
    if (sCol == dCol) {
        int col = sCol;
        int rowDif = dRow - sRow;
        //iterace mezi pozicemi
        for (int i = 1; i <= abs(rowDif); i++) {
            int row;
            if (rowDif > 0) {
                row = sRow + i;
            } else {
                row = sRow - i;
            }
            // Occupied
            if (field[row][col]->getPiece() != NULL) {
                if(row!=dRow || col!=dCol){
                    return false;
                }
            }
        }
        return true;
        // Column move
    } else if (sRow == dRow) {
        int row = sRow;
        int colDif = dCol - sCol;
        //iterace mezi pozicemi
        for (int i = 1; i <= abs(colDif); i++) {
            int col;
            if (colDif > 0) {
                col = sCol + i;
            } else {
                col = sCol - i;
            }
            
            // Occupied
            if (field[row][col]->getPiece() != NULL) {
                if(row!=dRow || col!=dCol){
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}


//Pawn Moves
bool Game::checkPawnMove(int sRow, int sCol, int dRow, int dCol){
    if(field[sRow][sCol]->getPiece()->getColor() == COLOR::WHITE){
      return checkWhitePawnMove(sRow, sCol, dRow, dCol);
    }
    else return checkBlackPawnMove(sRow, sCol, dRow, dCol);
}

bool Game::checkWhitePawnMove(int sRow, int sCol, int dRow, int dCol){
    //common move
    if(dRow - sRow == 1 ){
        if( sCol == dCol
           && field[dRow][dCol]->getPiece() == NULL){
            return true;
        }
        //capture
        else if( abs(sCol-dCol) == 1
                && field[dRow][dCol]->getPiece() != NULL){
            return true;
        }
        else {
            return false;
        }
    }
    else if((dRow - sRow == 2)
            && (sRow == 2)
            && (sCol == dCol)){
        if(field[dRow][dCol]->getPiece() == NULL
           && field[dRow-1][dCol]->getPiece() == NULL){
            return true;
        } else {
            return false;
        }
    }
    else {
        return false;
    }
    return false;
}
bool Game::checkBlackPawnMove(int sRow, int sCol, int dRow, int dCol){
    //common move
    if(sRow - dRow == 1 ){
        if( sCol == dCol
           && field[dRow][dCol]->getPiece() == NULL){
            return true;
        }
        //capture
        else if( abs(sCol-dCol) == 1
                && field[dRow][dCol]->getPiece() != NULL){
            return true;
        }
        else {
            return false;
        }
    }
    else if(sRow - dRow == 2
            && sRow == 7
            && sCol == dCol){
        if(field[dRow][dCol]->getPiece() == NULL
           && field[dRow+1][dCol]->getPiece() == NULL){
            return true;
        } else {
            return false;
        }
    }
    else {
        return false;
    }
}


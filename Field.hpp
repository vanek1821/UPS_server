//
//  Field.hpp
//  server
//
//  Created by Jakub  Vaněk on 11/12/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef Field_hpp
#define Field_hpp

#include <stdio.h>
#include "Piece.hpp"

class Field{
public:
    Field();
    void setPiece(Piece* piece) {this->piece = piece;};
    Piece* getPiece() {return this->piece;};
private:
    Piece* piece;
};

#endif /* Field_hpp */

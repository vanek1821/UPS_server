//
//  Piece.hpp
//  server
//
//  Created by Jakub  Vaněk on 11/12/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef Piece_hpp
#define Piece_hpp

#include <stdio.h>
#include <iostream>
#include "Color.hpp"
#include "Type.hpp"

using namespace std;

class Piece{
public:
    Piece(TYPE type, COLOR color);
    COLOR getColor() { return this->color;};
    TYPE getType() { return this->type;};
private:
    COLOR color;
    TYPE type;
};

#endif /* Piece_hpp */

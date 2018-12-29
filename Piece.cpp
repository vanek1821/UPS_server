//
//  Piece.cpp
//  server
//
//  Created by Jakub  Vaněk on 11/12/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#include "Piece.hpp"
#include <stdio.h>
#include <iostream>

using namespace std;

Piece::Piece(TYPE type, COLOR color){
    this->type = type;
    this->color = color;
}

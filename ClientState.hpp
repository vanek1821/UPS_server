//
//  ClientState.hpp
//  server
//
//  Created by Jakub  Vaněk on 28/12/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef ClientState_hpp
#define ClientState_hpp

#include <stdio.h>

using namespace std;

enum class ClientState{
    START, LOBBY, ROOM, GAME, MOVE, MOVE_WAIT, REPLAY
};
#endif /* ClientState_hpp */

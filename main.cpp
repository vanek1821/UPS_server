//
//  main.cpp
//  server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#include <iostream>
#include "Server.hpp"

int main(int argc, const char * argv[]) {
    
    std::cout <<"Hello to server!\n";
    
    Server* server = new Server();
    
    server->init();
    
    server->listenConnections();
    return 0;
}

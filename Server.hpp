//
//  Server.hpp
//  Server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include <stdio.h>
#include "MessageManager.hpp"

class Server{

public:
    void init();
    void listenConnections();
    static void *checkConnected(void *args);
private:
    int server_sock, client_sock, return_value;
};

#endif /* Server_hpp */

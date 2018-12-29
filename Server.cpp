//
//  server.cpp
//  server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>
#include "Server.hpp"


using namespace std;

fd_set client_socks, tests;

void Server::init(){
    std::cout <<"Server is setting up" <<std::endl;
    
    struct sockaddr_in local_addr;
    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock <= 0)
    {
        printf("Socket ERR\n");
        return;
    }
    
    int param = 1;
    return_value = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&param, sizeof(int));
    
    if (return_value == -1){
        
        printf("setsockopt ERR\n");
    }
    
    
    memset(&local_addr, 0, sizeof(struct sockaddr_in));
    
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(10000);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    
    return_value = ::bind(server_sock, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in));
    
    if (return_value == 0)
        printf("Bind OK\n");
    else
    {
        printf("Bind ER\n");
        return;
    }
    
    return_value = listen(server_sock, 5);
    
    if (return_value == 0)
        printf("Listen OK\n");
    else
        printf("Listen ERR\n");
    
    
  
}
void Server::listenConnections(){
    
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len;
    
    FD_ZERO(&client_socks);
    FD_SET(server_sock, &client_socks);
    
    while(1)
    {
        
        int client_sock, fd;
        int a2read;
        int MESSAGE_LENGTH = 1024;
        char cbuf[MESSAGE_LENGTH];
        
        
        // Zkoprovani fd_set do noveho, stary je zničen selectem ( modifikuje ho )
        tests = client_socks;
        
        return_value = select(FD_SETSIZE, &tests, (fd_set*)NULL, (fd_set*)NULL, (struct timeval *)0);
        
        if (return_value < 0){
            printf("Select ERR\n");
            return;
        }
        
        //vynechavame stdin, stdou, stderr
        for (fd = 3; fd<FD_SETSIZE; fd++){
            
            // je dany socket v sade fd ze kterych lze cist?
            if(FD_ISSET(fd, &tests)){
                
                if (fd==server_sock){ //serverovy socket
                    
                    client_sock = accept(server_sock, (struct sockaddr *)&remote_addr, &remote_addr_len);
                    FD_SET(client_sock, &client_socks);
                    printf("Připojil se nový hráč\n");
                }
                
                else {// pokud je to klientsky socket, prijmeme data
                    ioctl(fd, FIONREAD, &a2read);
                    //mame co cist?
                    if(a2read > 0){
                        recv(fd, &cbuf, MESSAGE_LENGTH*sizeof(char), 0);
                        
                        
                        string msg(cbuf);                        
                        MessageManager::resolveMessage(msg, fd);
                        memset(cbuf, 0, sizeof(cbuf));
                        
                    }
                    else {//na socketu se stalo něco špatného
                        GameManager::moveToDisconnected(fd);
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        cout << "na socketu "<< fd <<" se něco stalo..Uzivatel byl presunut do neaktivnich hracu" << endl;
                    }
                }
                
            }
        }
        
    }

}
void *Server::checkConnected(void *args){
    int id = *(int*)args;
    Client* c = GameManager::findClient(id);
    string msg = "PING;";
    int i = 0;
    while(c->getExisting()){
        if(c->isConnected()){
            i=0;
            MessageManager::sendMessage(msg, c->getSocketID());
            c->setConnected(false);
            sleep(10);
        }
        else{
            i++;
            cout << "try to reconnect with "<< c->getName() << i << endl;
            if(i==120){
                GameManager::removeClientByID(c->getSocketID());
                break;
            }
            sleep(1);
        }
    }
    pthread_exit(0);
    return 0;
}


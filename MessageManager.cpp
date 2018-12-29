//
//  MessageManager.cpp
//  server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//
#include <pthread.h>
#include "MessageManager.hpp"



using namespace std;

void MessageManager::resolveMessage(string msg, int socket){
    char delimeter = ';';
    cout <<"Prijimam zpravu: "<< msg << endl;
    size_t i = msg.find(delimeter); // nalezeni pozice delimitru
    string tok = msg.substr(0, i); // vyparsovani casti stringu do ;
    msg = msg.substr(i+1); // zbytek zpravy
    
    if(strcmp(tok.c_str(), "PING") == 0){
        Client *c = GameManager::findClient(socket);
        c->setConnected(true);
    }
    else if(strcmp(tok.c_str(), "CONNECT") == 0){
        i = msg.find(delimeter);
        tok = msg.substr(0, i);
        msg = msg.substr(i+1);
        
        if(strcmp(tok.c_str(), "OK") == 0){
            i = msg.find(delimeter);
            tok = msg.substr(0, i);
            if(GameManager::connectClient(tok, socket)){
                msg = "CONNECT;OK;";
                MessageManager::sendMessage(msg, socket);
                pthread_create(&(GameManager::findClient(socket)->thread), NULL, &Server::checkConnected, &socket);
                GameManager::findClient(socket)->setState(ClientState::LOBBY);
            }
        }
        else if(strcmp(tok.c_str(), "FAIL") == 0){
            cout << "Připojení hráce se nezdařilo" << endl;
            close(socket);
        }
    }
    
    else if(strcmp(tok.c_str(), "CREATE_MATCH") == 0){
        
        int result = GameManager::createMatch(socket);
        
        if(result == 1){
            msg = "CREATE_MATCH;OK;";
            MessageManager::sendMessage(msg, socket);
            GameManager::findClient(socket)->setState(ClientState::ROOM);
            msg = "FOUND_MATCH;";
            msg += GameManager::findMatches();
            sendToAll(msg);
        }
        else if(result == -1){
            msg = "CREATE_MATCH;FAIL";
            MessageManager::sendMessage(msg, socket);
        }
    }
    
    else if(strcmp(tok.c_str(), "FIND_MATCH") == 0){
        
        msg = "FOUND_MATCH;";
        msg += GameManager::findMatches();
        MessageManager::sendMessage(msg, socket);
        
    }
    else if(strcmp(tok.c_str(), "JOIN_MATCH") == 0){
        i = msg.find(delimeter);
        tok = msg.substr(0,i);
        int result = GameManager::joinMatch(tok, socket);
        if(result > -1) {
            msg = "JOIN_MATCH;OK;";
            Match* match = GameManager::findMatch(result);
            sendMessage(msg, match->getClient1()->getSocketID());
            match->getClient1()->setTurn(true);
            match->getClient1()->setState(ClientState::GAME);
            sendMessage(msg, match->getClient2()->getSocketID());
            match->getClient2()->setTurn(false);
            match->getClient2()->setState(ClientState::GAME);
            msg = "FOUND_MATCH;";
            msg += GameManager::findMatches();
            sendToAll(msg);
        }
        else{
            msg = "JOIN_MATCH;FAIL;";
            sendMessage(msg, socket);
        }
        
    }
    else if(strcmp(tok.c_str(), "MOVE") == 0){
        int sRow, sCol, dRow, dCol;
        i = msg.find(delimeter);
        tok = msg.substr(0, i);
        msg = msg.substr(i+1);
        sRow = atoi(tok.c_str());
        
        i = msg.find(delimeter);
        tok = msg.substr(0, i);
        msg = msg.substr(i+1);
        sCol = atoi(tok.c_str());
        
        i = msg.find(delimeter);
        tok = msg.substr(0, i);
        msg = msg.substr(i+1);
        dRow = atoi(tok.c_str());
        
        i = msg.find(delimeter);
        tok = msg.substr(0, i);
        msg = msg.substr(i+1);
        dCol = atoi(tok.c_str());
        
        Match* m = GameManager::findMatch(GameManager::findClient(socket)->getMatchID());
        
        switch (m->getGame()->move(GameManager::findClient(socket), sRow, sCol, dRow, dCol)){
            
            case 0:
                msg = "MOVE;FAIL;";
                sendMessage(msg, socket);
                break;
            
            case 1:
                msg = "MOVE;OK;" + to_string(sRow) + ";" + to_string(sCol) + ";" + to_string(dRow) + ";" + to_string(dCol)+ ";";
                
                if(m->getClient1()->isTurn()) m->getClient1()->setTurn(false);
                else m->getClient1()->setTurn(true);
                
                if(m->getClient2()->isTurn()) m->getClient2()->setTurn(false);
                else m->getClient2()->setTurn(true);
                
                sendMessage(msg, m->getClient1()->getSocketID());
                sendMessage(msg, m->getClient2()->getSocketID());
                break;
                
            case 2:
                msg = "MOVE;OK;" + to_string(sRow) + ";" + to_string(sCol) + ";" + to_string(dRow) + ";" + to_string(dCol)+ ";";
                sendMessage(msg, m->getClient1()->getSocketID());
                sendMessage(msg, m->getClient2()->getSocketID());
                
                m->setReady(false);
                m->getClient1()->setInGame(false);
                m->getClient1()->setState(ClientState::REPLAY);
                m->getClient2()->setInGame(false);
                m->getClient2()->setState(ClientState::REPLAY);
                
                msg = "GAME;WIN;";
                sendMessage(msg, socket);
                msg = "GAME;LOST;";
                if(m->getClient1()->getSocketID() == socket){
                    sendMessage(msg, m->getClient2()->getSocketID());
                    m->getClient1()->setResult(true);
                    m->getClient2()->setResult(false);
                }
                else{
                    sendMessage(msg, m->getClient1()->getSocketID());
                    m->getClient2()->setResult(true);
                    m->getClient1()->setResult(false);
                }
                break;
        }
    }
    else if(strcmp(tok.c_str(), "REPLAY_Y") == 0){
        Match* m = GameManager::findMatch(GameManager::findClient(socket)->getMatchID());
         
        if(GameManager::replayMatch(socket)){
            msg = "REPLAY;YES;";
            sendMessage(msg, m->getClient1()->getSocketID());
            sendMessage(msg, m->getClient2()->getSocketID());
            m->getClient1()->setState(ClientState::GAME);
            m->getClient2()->setState(ClientState::GAME);
        }
    }
    else if(strcmp(tok.c_str(), "REPLAY_N") == 0){
        Match* m = GameManager::findMatch(GameManager::findClient(socket)->getMatchID());
        msg = "REPLAY;NO;";
        sendMessage(msg, m->getClient1()->getSocketID());
        sendMessage(msg, m->getClient2()->getSocketID());
        m->getClient1()->setState(ClientState::LOBBY);
        m->getClient2()->setState(ClientState::LOBBY);
        GameManager::leaveMatch(m->getID());
        msg = "FOUND_MATCH;";
        msg += GameManager::findMatches();
        sendToAll(msg);
    }
    else if(strcmp(tok.c_str(), "STOP_WAIT") == 0){
        Match* m = GameManager::findMatch(GameManager::findClient(socket)->getMatchID());
        GameManager::leaveMatch(m->getID());
        GameManager::findClient(socket)->setState(ClientState::LOBBY);
        msg = "FOUND_MATCH;";
        msg += GameManager::findMatches();
        sendToAll(msg);
    }
    else if(strcmp(tok.c_str(), "YIELD") == 0){
        Match* m = GameManager::findMatch(GameManager::findClient(socket)->getMatchID());
        msg = "YIELD;";
        sendMessage(msg, m->getClient1()->getSocketID());
        sendMessage(msg, m->getClient2()->getSocketID());
        m->getClient1()->setState(ClientState::LOBBY);
        m->getClient2()->setState(ClientState::LOBBY);
        GameManager::leaveMatch(m->getID());
        msg = "FOUND_MATCH;";
        msg += GameManager::findMatches();
        sendToAll(msg);
    }
}


void MessageManager::sendMessage(string msg, int socket){
    msg += "\n";
    cout << "Odesilam zpravu: " << msg;
    send(socket, msg.data(), msg.length(), 0);
}

void MessageManager::sendToAll(string msg){
    map<int, Client*> myMap = GameManager::getClientMap();
    map<int, Client*>::iterator it;
    for(it = myMap.begin(); it != myMap.end(); it++){
        sendMessage(msg, it->first);
    }
}

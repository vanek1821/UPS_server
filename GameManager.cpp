//
//  GameManager.cpp
//  server
//
//  Created by Jakub  Vaněk on 22/11/2018.
//  Copyright © 2018 Jakub  Vaněk. All rights reserved.
//

#include "GameManager.hpp"

map<int, Client*> GameManager::clientMap;
map<int, Match*> GameManager::matchMap;
map<string, Client*> GameManager::disconnectedMap;

bool GameManager::connectClient(string name, int socket){
    if(GameManager::clientExists(name, socket)){
        reconnectClient(socket);
        return false;
    }
    else{
        Client* client = new Client(name, socket);
        addToClientMap(client);
        return true;
    }
    
}

void GameManager::addToClientMap(Client* client){
    
    clientMap.insert(make_pair(client->getSocketID(), client));
    
    std::cout << "Klient " << client->getName() << " se socketem " << client->getSocketID() << " byl přidán do mapy." << endl;
}

int GameManager::createMatch(int clientID){
    map<int, Client*>::iterator it = clientMap.find(clientID);
    if(it != clientMap.end()){
        Client* client = it->second;
        
        cout << "Hráč " << client->getName() << " chce vytvořit hru." << endl;
        
        Match* match = new Match(client->getSocketID());
        
        cout << "Hra " << client->getSocketID() << " byla vytvořena" << endl;
        
        matchMap.insert(make_pair(client->getSocketID(), match));
        match->setClient1(client);
        client->setMatchID(match->getID());
        return 1;
    }
    else{
        return -1;
    }
}

string GameManager::findMatches(){
    string msg;
    for(map<int,Match*>::iterator it = matchMap.begin(); it != matchMap.end(); ++it) {
        if(it->second->isReady() == false){
            cout << "Hra s ID: "<< it->first << " nalezena" << endl;
            msg = msg + to_string(it->first);
            msg = msg + "-";
        }
    }
    msg = msg.substr(0, msg.length()-1); //odebrani posledni pomlcky
    cout << "Pridavam ke zprave: " << msg << endl;
    return msg;
}

int GameManager::joinMatch(string matchS, int clientID){
    Match* match = GameManager::findMatch(atoi(matchS.c_str()));
    if(match == NULL) return -1;
    if(match->isReady()) return -1;
    Client* client = GameManager::findClient(clientID);
    if(match == NULL) return -1;
    match->setClient2(client);
    client->setMatchID(match->getID());
    match->setReady(true);
    GameManager::initGame(match->getID());
    match->getClient1()->setColor(COLOR::WHITE);
    match->getClient2()->setColor(COLOR::BLACK);
    
    return match->getID();
}

Client* GameManager::findClient(int clientID){
    map<int, Client*>::iterator it = clientMap.find(clientID);
    if(it != clientMap.end()){
        Client* client = it->second;
        return client;
    }
    else{
        cout << "hráč s ID: " << clientID << " nenalezen.";
        return NULL;
    }
}
Client* GameManager::findClientByName(string name){
    map<int, Client*>::iterator it;
    for(it=clientMap.begin(); it != clientMap.end(); it++){
        if(strcmp(it->second->getName().c_str(), name.c_str()) == 0){
            return it->second;
        }
    }
    return NULL;
}

void GameManager::initGame(int matchID){
    Game* game = new Game();
    Match* match = findMatch(matchID);
    match->setGame(game);
}

Match* GameManager::findMatch(int matchID){
    map<int, Match*>::iterator it = matchMap.find(matchID);
    if(it != matchMap.end()){
        Match* match = it->second;
        return match;
    }
    else{
        cout << "Match s ID: " << matchID << " nenalezen.";
        return NULL;
    }
}

bool GameManager::replayMatch(int socket){
    Client* c = GameManager::findClient(socket);
    c->setInGame(true);
    Match* m = GameManager::findMatch(c->getMatchID());
    
    if(m->getClient1()->getInGame() && m->getClient2()->getInGame()){
        m->setReady(true);
    }
    if(m->isReady()){
        GameManager::initGame(m->getID());
        m->getClient1()->setTurn(true);
        m->getClient2()->setTurn(false);
        return true;
    }
    return false;
}

void GameManager::leaveMatch(int matchID){
    Match* m = GameManager::findMatch(matchID);
    if(m->getClient1()!=NULL){
        m->getClient1()->setMatchID(-1);
    }
    if(m->getClient2()!= NULL){
        m->getClient2()->setMatchID(-1);
    }
    m->setClient1(NULL);
    m->setClient2(NULL);
    m->setGame(NULL);
    matchMap.erase(matchID);
    cout << "Hra s ID: " << matchID << " byla zničena." << endl;
}

void GameManager::removeClientByID(int socketID){
    cout<< "Mažu klienta " << endl;
    Client *c = GameManager::findClient(socketID);
    Match *m;
    string msg;
    
    if(c==NULL){
        return;
    }
    
    if(c->getState()==ClientState::LOBBY){
        clientMap.erase(c->getSocketID());
        c->setExisting(false);
        delete(c);
    }
    
    else if(c->getState()==ClientState::ROOM){
        m = GameManager::findMatch(c->getMatchID());
        m->setGame(NULL);
        m->setClient1(NULL);
        delete(m);
        matchMap.erase(c->getMatchID());
        c->setExisting(false);
        clientMap.erase(c->getSocketID());
        delete(c);
        msg = "FOUND_MATCH;";
        msg += GameManager::findMatches();
        MessageManager::sendToAll(msg);
    }
    
    else if(c->getState()==ClientState::GAME){
        m = GameManager::findMatch(c->getMatchID());
        msg = "LEFT;";
        if(m->getClient1()->getSocketID() == socketID){
            MessageManager::sendMessage(msg, m->getClient2()->getSocketID());
            m->getClient2()->setState(ClientState::LOBBY);
        }
        else if(m->getClient2()->getSocketID() == socketID){
            MessageManager::sendMessage(msg, m->getClient1()->getSocketID());
            m->getClient1()->setState(ClientState::LOBBY);
        }
        delete(m->getGame());
        delete(m);
        matchMap.erase(c->getMatchID());
        c->setExisting(false);
        clientMap.erase(c->getSocketID());
        delete(c);
        msg = "FOUND_MATCH;";
        msg += GameManager::findMatches();
        MessageManager::sendToAll(msg);
    }
    
    else if(c->getState()==ClientState::REPLAY){
        m = GameManager::findMatch(c->getMatchID());
        msg = "REPLAY;NO;";
        if(m->getClient1()->getSocketID() == socketID){
            MessageManager::sendMessage(msg, m->getClient2()->getSocketID());
            m->getClient2()->setState(ClientState::LOBBY);
        }
        else if(m->getClient2()->getSocketID() == socketID){
            MessageManager::sendMessage(msg, m->getClient1()->getSocketID());
            m->getClient1()->setState(ClientState::LOBBY);
        }
        delete(m->getGame());
        delete(m);
        matchMap.erase(c->getMatchID());
        c->setExisting(false);
        clientMap.erase(c->getSocketID());
        delete(c);
        msg = "FOUND_MATCH;";
        msg += GameManager::findMatches();
        MessageManager::sendToAll(msg);
    }
}

bool GameManager::clientExists(string name, int socket){
    map<string, Client*>::iterator it = disconnectedMap.find(name);
    if(it != disconnectedMap.end()){
        Client* c = it->second;
        clientMap.insert(make_pair(socket, c));
        c->setConnected(true);
        c->setSocketID(socket);
        disconnectedMap.erase(name);
        return true;
    }
    return false;
}

void GameManager::reconnectClient(int socket){
    string msg = "RECONNECT;";
    Client *c = GameManager::findClient(socket);
    c->setConnected(true);
    if(c->getState()==ClientState::LOBBY){
        msg += "LOBBY;";
        MessageManager::sendMessage(msg, socket);
    }
    else if(c->getState()==ClientState::ROOM){
        msg += "LOBBY;";
        MessageManager::sendMessage(msg, socket);
    }
    else if(c->getState()==ClientState::GAME){
        msg += "GAME;";
        if(c->isTurn()) msg += "Y;";
        else msg += "N;";
        MessageManager::sendMessage(msg, socket);
        sleep(1);
        GameManager::sendGame(socket);
    }
    else if(c->getState()==ClientState::REPLAY){
        msg += "REPLAY;";
        if(c->getResult()) msg += "W;";
        else msg += "L";
        MessageManager::sendMessage(msg, socket);
    }
}

void GameManager::moveToDisconnected(int socketID){
    map<int, Client*>::iterator it = clientMap.find(socketID);
    if(it != clientMap.end()){
        Client* c = it->second;
        c->setConnected(false);
        clientMap.erase(it->first);
        disconnectedMap.insert(make_pair(c->getName(), c));
        if(c->getState()==ClientState::ROOM){
            map<int, Match*>::iterator mit = matchMap.find(socketID);
            if(mit != matchMap.end()){
                Match* m = mit->second;
                matchMap.erase(it->first);
                delete(m);
                string msg = "FOUND_MATCH;";
                msg += GameManager::findMatches();
                MessageManager::sendToAll(msg);
            }
        }
    }
}

void GameManager::sendGame(int socket) {
    Match* m = findMatch(findClient(socket)->getMatchID());
    Game* g = m->getGame();
    string msg;
    for (int i = 1; i<=8; i++) {
        for(int j = 1; j<=8; j++){
            if(g->field[i][j]->getPiece()!=NULL){
                msg = "PIECE;";
                msg += to_string(i);
                msg += "-";
                msg += to_string(j);
                msg += "-";
                if(g->field[i][j]->getPiece()->getColor()==COLOR::WHITE){
                    msg += "W";
                }
                else{
                    msg += "B";
                }
                msg += "-";
                if(g->field[i][j]->getPiece()->getType() == TYPE::QUEEN){
                    msg += "Q;";
                }
                else if(g->field[i][j]->getPiece()->getType() == TYPE::KING){
                    msg += "K;";
                }
                else if(g->field[i][j]->getPiece()->getType() == TYPE::PAWN){
                    msg += "P;";
                }
                else if(g->field[i][j]->getPiece()->getType() == TYPE::BISHOP){
                    msg += "B;";
                }
                else if(g->field[i][j]->getPiece()->getType() == TYPE::ROOK){
                    msg += "R;";
                }
                else if(g->field[i][j]->getPiece()->getType() == TYPE::KNIGHT){
                    msg += "N;";
                }
                MessageManager::sendMessage(msg, socket);
            }
        }
    }
    
}

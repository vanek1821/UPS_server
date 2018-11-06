#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "client.h"

/* creates new client
*/
Client *createClient(char *name, int socket){
	Client *tmp = NULL;
	tmp = malloc(sizeof(Client));
	tmp->socketID = socket;
	tmp->name = malloc(sizeof(strlen(name) + 1));
	strcpy(tmp->name, name);
	tmp->state = 0;
	tmp->inGame = false;
	return(tmp);
}

void addToClients(Client ** clients, Client * client){
	//clients = realloc(clients, sizeof(Client*)*client->socketID + 1);
}
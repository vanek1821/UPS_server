#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "client.h"
#include "clientPool.h"


/*creates new clientpool
*/
ClientPool *createClientPool(){
	ClientPool *tmp = NULL;
	tmp = malloc(sizeof(ClientPool));
	tmp->pool = malloc(sizeof(Client*));
	tmp->poolSize = 1;
	tmp->pointer = 0;
	return tmp; 
}
void addToClientPool(ClientPool *cp, Client *c){
	cp->pool = realloc(cp->pool, sizeof(Client*) * (cp->poolSize+1));
	cp->pool[cp->pointer] = (Client*) c;
	cp->pointer++;
	cp->poolSize = cp->pointer;
}

Client *findClient(ClientPool *cp, int socket){
	Client *tmp = NULL;
	for (int i = 0; i < cp->poolSize; ++i)
	{
		tmp = cp->pool[i];
		if(socket == tmp->socketID){
			return tmp;
		}
	}
	return NULL;
}
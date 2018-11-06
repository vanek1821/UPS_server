#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "client.h"
#include "match.h"

/*creates new clientpool
*/
Match *createMatch(short id, Client *c1){
	Match *tmp = NULL;
	tmp = malloc(sizeof(Match));
	tmp->ready = false;
	tmp->matchID = id;
	//tmp->c1 = malloc(sizeof(Client*));
	tmp->c1 = c1;
	tmp->c2 = NULL;
	return tmp; 
}
void addToMatch(Match *m, Client *c2){
	m->c2 = c2;
	m->ready = true;
}
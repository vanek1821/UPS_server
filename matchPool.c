#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "client.h"
#include "match.h"
#include "matchPool.h"


/*creates new clientpool
*/
MatchPool *createMatchPool(){
	MatchPool *tmp = NULL;
	tmp = malloc(sizeof(MatchPool));
	tmp->pool = malloc(sizeof(Match*));
	tmp->poolSize = 1;
	tmp->pointer = 0;
	return tmp; 
}
void addToMatchPool(MatchPool *mp, Match *m){
	mp->pool = realloc(mp->pool, sizeof(Match*) * (mp->poolSize+1));
	mp->pool[mp->pointer] = (Match*) m;
	mp->pointer++;
	mp->poolSize = mp->pointer;
}

Match *findMatch(MatchPool *mp, int pointer){
	Match *tmp = NULL;
	printf("hledam match");
	if (mp->pool[pointer]->ready == false){
		tmp = mp->pool[pointer];
		return tmp;
	}
	return NULL;
}
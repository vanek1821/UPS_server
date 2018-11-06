typedef struct matchpo MatchPool;

struct matchpo{
	short poolSize;
	Match **pool;
	short pointer;
};

MatchPool *createMatchPool();
void addToMatchPool(MatchPool *mp, Match *m);
Match *findMatch(MatchPool *mp, int pointer);
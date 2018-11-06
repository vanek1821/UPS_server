typedef struct ma Match;

struct ma{
	bool ready;
	short matchID;
	Client *c1;
	Client *c2;
};

Match *createMatch(short id, Client *c1);
void addToMatch(Match *m, Client *c2);
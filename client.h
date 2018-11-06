typedef struct cli Client;

struct cli{
	int socketID;
	char *name;
	int state;
	bool inGame;

};

Client *createClient(char *name, int socket);

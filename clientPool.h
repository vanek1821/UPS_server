typedef struct clipo ClientPool;

struct clipo{
	short poolSize;
	Client **pool;
	short pointer;
};

ClientPool *createClientPool();
void addToClientPool(ClientPool *cp, Client *c);
Client *findClient(ClientPool *cp, int socket);
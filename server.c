#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>
#include "client.h"
#include "clientPool.h"
#include "match.h"
#include "matchPool.h"

int main (void)
{
	int server_sock;
	int client_sock, fd;
	int return_value;
	int MESSAGE_LENGTH = 1024;
	int SUPPOSED_PLAYERS_COUNT = 10;
	int a2read;
	char cbuf[MESSAGE_LENGTH];
	struct sockaddr_in local_addr;
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len;
	ClientPool * clientPool = createClientPool();
	MatchPool * matchPool = createMatchPool();
	int client_id = 0;
	fd_set client_socks, tests;


	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock <= 0)
	{
		printf("Socket ERR\n");
		return -1;
	}

	int param = 1;
	return_value = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&param, sizeof(int));

	if (return_value == -1){
	
		printf("setsockopt ERR\n");
	}
	
	
	memset(&local_addr, 0, sizeof(struct sockaddr_in));

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(10000);
	local_addr.sin_addr.s_addr = INADDR_ANY;

	return_value = bind(server_sock, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in));

	if (return_value == 0)
		printf("Bind OK\n");
	else
	{
		printf("Bind ER\n");
		return -1;
	}

	return_value = listen(server_sock, 5);

	if (return_value == 0)
		printf("Listen OK\n");
	else
		printf("Listen ERR\n");

	FD_ZERO(&client_socks);
	FD_SET(server_sock, &client_socks);

	while(1)
	{
		// Zkoprovani fd_set do noveho, stary je zničen selectem ( modifikuje ho )
		tests = client_socks;

		return_value = select(FD_SETSIZE, &tests, (fd_set*)NULL, (fd_set*)NULL, (struct timeval *)0);

		if (return_value < 0){
			printf("Select ERR\n");
			return -1;
		}

		//vynechavame stdin, stdou, stderr
		for (fd = 3; fd<FD_SETSIZE; fd++){

			// je dany socket v sade fd ze kterych lze cist?
			if(FD_ISSET(fd, &tests)){

				//je to server socket?
				if (fd==server_sock){

					client_sock = accept(server_sock, (struct sockaddr *)&remote_addr, &remote_addr_len);			
					FD_SET(client_sock, &client_socks);
					printf("Připojil se nový hráč\n");	
				}

				else {// pokud je to klientsky socket, prijmeme data
					ioctl(fd, FIONREAD, &a2read);
					//mame co cist?
					if(a2read > 0){

						recv(fd, &cbuf, MESSAGE_LENGTH*sizeof(char), 0);
						Client * tmp = NULL;
						Match * m = NULL;
						printf("něco jsem přijal %s\n", cbuf);

						char * tok;
						tok = strtok(cbuf, ";");
						printf("porovnávám: %s\n", tok);
						
						//printf("Chci porovnat: %s\n", tok);

						if(strcmp(tok, "CONNECT") == 0){
							tok = strtok(NULL, ";");
							if(strcmp(tok, "OK")==0){
								tok = strtok(NULL,";");							
								tmp = createClient(tok, fd);
								addToClientPool(clientPool, tmp);
								tmp = findClient(clientPool, fd);
								printf("Připojen\nHráč: %s\nsocketID: %d\n", tmp->name, tmp->socketID);
								char * message = "CONNECT;OK;";
								send(fd, message , strlen(message)*sizeof(char), 0);
								tmp->state=1;
							}
							else if(strcmp(tok, "FAIL")==0){
								printf("Připojení hráče se nezdařilo");
								close(client_sock);
							}
						}
						else if (strcmp(tok, "CREATE_MATCH") == 0){
							//printf("Jdu najít hráče s id: %d\n", fd);
							tmp = findClient(clientPool, fd);
							if(tmp->inGame == false){
								printf("Hráč %s chce vytvořit hru\n", tmp->name);
								tmp->state = 2;
								m = createMatch(matchPool->pointer, tmp);
								addToMatchPool(matchPool, m);
								tmp->inGame = true;
								char * message = "CREATE_MATCH;OK;";
								send(fd, message, strlen(message)*sizeof(char), 0);
								printf("Hra vytvořena\n");
							}else{
								printf("Hráč %s je již ve hře\n",tmp->name);
							}
						}
						else if (strcmp(tok, "FIND_MATCH") == 0){
							printf("jdu hledat matche");
							int i = 0;
							
							for (int i = 0; i < matchPool->poolSize; i++)
							{
								m = findMatch(matchPool, i);
								if(m!=NULL){
									char *message = "FOUND_MATCH;";
									char id = (char)m->matchID;
									strcat(message, &id);
									strcat(message, ";");
									printf("Chci poslat zprávu: %s\n", message);
								}
							}
						}
						else{
							printf("přijal jsem neznámou zprávu\n");
						}
					} 
					else {//na socketou se stalo něco spatneho
						close(fd);
						FD_CLR(fd, &client_socks);
						printf("klient %d se odpojil a byl odebran ze sady socketu\n", fd);
					}	
				}

			}
		}

	}

return 0;
}

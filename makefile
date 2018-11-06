CC = gcc
BIN = chess_server.exe
OBJ = client.o clientPool.o match.o matchPool.o server.o

all: $(BIN) clean

$(BIN): $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@
  
clean:	
	rm -f *.o
CC = gcc
C_SRC = client.c
S_SRC = server.c
C_OUT = bc_client.out
S_OUT = bc_server.out

all: client server

client: $(C_SRC)
	$(CC) $(C_SRC) -o $(C_OUT)

server: $(S_SRC)
	$(CC) $(S_SRC) -o $(S_OUT)

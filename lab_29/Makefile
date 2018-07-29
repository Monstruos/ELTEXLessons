CC = gcc
C_SRC = raw_client.c
S_SRC = server.c
C_OUT = raw_client.out
S_OUT = server.out

all: client server

client: $(C_SRC)
	$(CC) -g $(C_SRC) -o $(C_OUT)

server: $(S_SRC)
	$(CC) -g $(S_SRC) -o $(S_OUT)

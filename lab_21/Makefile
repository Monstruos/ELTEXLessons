CC = gcc
C_SRC = client.c
S_SRC = server.c
DEF_SRC = msgchat.c

C_OUT = client.out
S_OUT = server.out

all: client server

client: $(C_SRC)
	$(CC) -g $(C_SRC) $(DEF_SRC) -pthread -o $(C_OUT)

server: $(S_SRC)
	$(CC) -g $(S_SRC) $(DEF_SRC) -o $(S_OUT)

CC = gcc
C_SRC = tcp_client.c
S_SRC = tcp_server.c
C_OUT = tcp_client.out
S_OUT = tcp_server.out

all: client server

client: $(C_SRC)
	$(CC) $(C_SRC) -o $(C_OUT)

server: $(S_SRC)
	$(CC) $(S_SRC) -o $(S_OUT)

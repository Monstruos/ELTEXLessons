CC = gcc
C_SRC = udp_client.c
S_SRC = udp_server.c
C_OUT = udp_client.out
S_OUT = udp_server.out

all: client server

client: $(C_SRC)
	$(CC) $(C_SRC) -o $(C_OUT)

server: $(S_SRC)
	$(CC) $(S_SRC) -o $(S_OUT)

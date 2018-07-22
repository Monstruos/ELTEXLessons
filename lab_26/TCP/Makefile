CC = gcc
C_SRC = tester.c
S_SRC = server.c
C_OUT = tcp_tester.out
S_OUT = tcp_server.out

all: tester server

tester: $(C_SRC)
	$(CC) $(C_SRC) -o $(C_OUT)

server: $(S_SRC)
	$(CC) $(S_SRC) -pthread -o $(S_OUT)

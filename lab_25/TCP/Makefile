CC = gcc
S_SRC = tcp_server.c
S_OUT = tcp_server.out
TEST_SRC = tcp_tester.c
TEST_OUT = test.out

all: server test

server: $(S_SRC)
	$(CC) $(S_SRC) -o $(S_OUT)

test: $(TEST_SRC)
	$(CC) $(TEST_SRC) -pthread -o $(TEST_OUT)

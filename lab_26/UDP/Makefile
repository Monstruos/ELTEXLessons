CC = gcc
S_SRC = server.c
S_OUT = udp_server.out
S_O = $(S_SRC:.c=.o)
TEST_SRC = tester.c
TEST_OUT = udp_tester.out


all: server test

server: $(S_SRC)
	$(CC) $(S_SRC) -o $(S_OUT)

test: $(TEST_SRC)
	$(CC) $(TEST_SRC) -o $(TEST_OUT)

CC = gcc
P_SRC = ping.c
P_OUT = ping.out
T_SRC = traceroute.c
T_OUT = traceroute.out

all: ping trace

ping: $(P_SRC)
	$(CC) -g $(P_SRC) -o $(P_OUT)

trace: $(T_SRC)
	$(CC) -g $(T_SRC) -o $(T_OUT)

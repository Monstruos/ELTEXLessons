CC = gcc
SEL_SRC = select_server.c
POLL_SRC = poll_server.c
EPOLL_SRC = epoll_server.c
S_OUT = multiserver.out
UDP_SRC = udp_tester.c
UDP_OUT = udp_test.out
TCP_SRC = tcp_tester.c
TCP_OUT = tcp_test.out

select: clean sel_server test_udp test_tcp
poll: clean poll_server test_udp test_tcp
epoll: clean epoll_server test_udp test_tcp

sel_server: $(SEL_SRC)
	$(CC) -g $(SEL_SRC) -o $(S_OUT)

poll_server: $(POLL_SRC)
	$(CC) -g $(POLL_SRC) -o $(S_OUT)

epoll_server: $(EPOLL_SRC)
	$(CC) -g $(EPOLL_SRC) -o $(S_OUT)

test_udp: $(UDP_SRC)
	$(CC) $(UDP_SRC) -o $(UDP_OUT)

test_tcp: $(TCP_SRC)
	$(CC) $(TCP_SRC) -o $(TCP_OUT)

clean:
	rm -rf *.out

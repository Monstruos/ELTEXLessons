#Build: make build
#Compile: make all
#Compile and run in debugger: make all && make run-debug
CC = gcc
SRC = main.c recur.c
OBJ = $(SRC:.c=.o)
RUN = lab_11.out

.PHONY: all clean

all: $(OBJ)
	$(CC) $(OBJ) -o $(RUN)

build: clean all

%.o: %.c
	$(CC) -c -g $(SRC)

clean:
	rm -rf *.o

run-debug: $(RUN)
	gdb $(RUN)

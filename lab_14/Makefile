#Build: make build
#Compile: make all
#Run: make run
CC = gcc
CALC_SRC = add.c sub.c multi.c division.c
CALC_OBJ = $(CALC_SRC:.c=.o)
SRC = main.c
OBJ = $(SRC:.c=.o)
LIB = calc
RUN = lab_13.out

.PHONY: all clean build run

all: $(OBJ) $(LIB)
	$(CC) $(OBJ) -L. -l$(LIB) -o $(RUN)

build: clean all

$(CALC_OBJ): $(CALC_SRC)
	$(CC) -c $(CALC_SRC)

$(OBJ): $(SRC)
	$(CC) -c $(SRC)

$(LIB): $(CALC_OBJ)
	ar rc lib$(LIB).a $(CALC_OBJ)

clean:
	rm -rf *.o *.a

run: $(RUN)
	./$(RUN)

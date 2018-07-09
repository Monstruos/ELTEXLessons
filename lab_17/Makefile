CC = gcc
SRC = main.c
LIB = malloc
LIBSRC = malloc.c
LIBOBJ = malloc.o
OUT = lab_17.out

all: libmalloc
	$(CC) $(SRC) -l$(LIB) -L. -o $(OUT)

libmalloc:
	$(CC) -fPIC -c $(LIBSRC)
	$(CC) -shared $(LIBOBJ) -o lib$(LIB).so

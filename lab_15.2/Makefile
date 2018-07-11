#For running this makefile you should type:
#make [commands]
#
#Commands:
#Build: build
#Compile: all
#
#Before running you should set LD_LIBRARY_PATH variable in your enviroment:
#LD_LIBRARY_PATH=<full path to this directory> (export LD_LIBRARY_PATH=/...)
#All plugins (*.so) you should set into ./plugins/so directory

CC = gcc
SRC = main.c plugin_reader.c
OBJ = $(SRC:.c=.o)
RUN = lab_15_plug.out

.PHONY: all clean build run calcso

all: $(OBJ)
	$(CC) $(OBJ) -o $(RUN) -ldl

build: clean all

clean:
	rm -rf *.o *.so

#$(OBJ): $(SRC)
#	$(CC) -c $(SRC)

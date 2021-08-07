CC=gcc
INCLUDES= -I ./include
FLAGS= -g
OBJFILES= main.o display.o chip8.o
all: 
	gcc ${FLAGS} ${INCLUDES} ./src/main.c ./src/chip8.c ./src/display.c -lSDL2 -o ./bin/main -Wall
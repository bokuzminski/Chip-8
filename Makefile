INCLUDES= -I ./include
FLAGS= -g
all: 
	gcc ${FLAGS} ${INCLUDES} ./src/main.c -lSDL2 -o  ./bin/main
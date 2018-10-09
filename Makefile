NAME=cubesim
CC=g++
CFLAGS=-c -g -Wall -Wextra -pedantic
LIBS=-lGL -lGLU -lglfw
OBJ=cube.o inputhandler.o main.o viewport.o

default: $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIBS)

cube.o: cube.cpp cube.hpp
	$(CC) $(CFLAGS) cube.cpp

inputhandler.o: inputhandler.cpp viewport.hpp
	$(CC) $(CFLAGS) inputhandler.cpp

main.o: main.cpp viewport.hpp
	$(CC) $(CFLAGS) main.cpp

viewport.o: viewport.cpp viewport.hpp cube.hpp
	$(CC) $(CFLAGS) viewport.cpp

clean:
	rm -f *.o $(NAME)

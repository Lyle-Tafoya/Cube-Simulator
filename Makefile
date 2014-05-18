NAME=cubesim
CC=g++
CFLAGS=-c -Wall -Wextra -pedantic
LIBS=-lGL -lGLU -lsfml-graphics -lsfml-system -lsfml-window 
OBJ=cube.o display.o input.o main.o

default: $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIBS)

cube.o: cube.cpp cube.h
	$(CC) $(CFLAGS) cube.cpp

display.o: display.cpp display.h cube.h
	$(CC) $(CFLAGS) display.cpp

input.o: input.cpp display.h
	$(CC) $(CFLAGS) input.cpp

main.o: main.cpp display.h
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -f *.o $(NAME)

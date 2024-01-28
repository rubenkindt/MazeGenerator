app: maze.cpp maze.o maze.h
	cc -o app maze.o

maze.o : maze.cpp maze.h
	cc -c maze.cpp

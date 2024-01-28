app: maze.cpp maze.0 maze.h
    cc -o app maze.o

maze.o : maze.cpp maze.h
	cc -c maze.cpp
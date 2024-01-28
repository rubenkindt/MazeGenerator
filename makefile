cc = g++
app: maze.cpp maze.o maze.h
	${cc} maze.o -o app

maze.o: maze.cpp maze.h
	${cc} -c maze.cpp
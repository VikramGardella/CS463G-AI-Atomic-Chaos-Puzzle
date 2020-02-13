SOURCES = main.cpp Node.cpp
OBJECTS = main.o Node.o Heap.o


puzzle: main.cpp Node.cpp
	g++ -o puzzle main.cpp Node.cpp

CXXFLAGS = -std=c++11 -D_GNU_SOURCE -Wall

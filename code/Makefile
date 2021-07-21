FLAGS= -Wall -std=c++17

all: main board node functions
	g++ ${FLAGS} -o main.out main.o node.o board.o functions.o

main: main.cpp
	g++ ${FLAGS} -o main.o -c main.cpp

board: board.cpp
	g++ ${FLAGS} -o board.o -c board.cpp

node: node.cpp
	g++ ${FLAGS} -o node.o -c node.cpp

functions: functions.cpp
	g++ ${FLAGS} -o functions.o -c functions.cpp
	
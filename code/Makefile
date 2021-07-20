FLAGS= -Wall -std=c++17

all: main node functions
	g++ ${FLAGS} -o main.out main.o node.o functions.o

main: main.cpp
	g++ ${FLAGS} -o main.o -c main.cpp

node: node.cpp
	g++ ${FLAGS} -o node.o -c node.cpp

functions: functions.cpp
	g++ ${FLAGS} -o functions.o -c functions.cpp
	
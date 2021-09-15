FLAGS= -Wall -std=c++17 -Wshadow -Wextra -Wno-unused-parameter -O2

all: main.o position.o functions.o generators.o move.o node.o movecheck.o
	g++ ${FLAGS} -o PieceBreaker main.o position.o functions.o generators.o move.o node.o movecheck.o

main.o: main.cpp
	g++ ${FLAGS} -o main.o -c main.cpp

position.o: position.cpp position.h
	g++ ${FLAGS} -o position.o -c position.cpp

functions.o: functions.cpp functions.h
	g++ ${FLAGS} -o functions.o -c functions.cpp

generators.o: movegenerators.cpp movegenerators.h
	g++ ${FLAGS} -o generators.o -c movegenerators.cpp

move.o: move.cpp move.h
	g++ ${FLAGS} -o move.o -c move.cpp

node.o: node.cpp node.h
	g++ ${FLAGS} -o node.o -c node.cpp

movecheck.o: movecheck.cpp movecheck.h
	g++ ${FLAGS} -o movecheck.o -c movecheck.cpp

.PHONY: clean

clean:
	rm -f *.o PieceBreaker

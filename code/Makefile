FLAGS= -Wall -std=c++17 -Wshadow -Wextra -Wno-unused-parameter -O2

all: main position functions movegen move node movecheck
	g++ ${FLAGS} -o PieceBreaker main.o position.o functions.o generators.o move.o node.o movecheck.o

main: main.cpp
	g++ ${FLAGS} -o main.o -c main.cpp

position: position.cpp
	g++ ${FLAGS} -o position.o -c position.cpp

functions: functions.cpp
	g++ ${FLAGS} -o functions.o -c functions.cpp

movegen: movegenerators.cpp
	g++ ${FLAGS} -o generators.o -c movegenerators.cpp

move: move.cpp
	g++ ${FLAGS} -o move.o -c move.cpp

node: node.cpp
	g++ ${FLAGS} -o node.o -c node.cpp

movecheck: movecheck.cpp
	g++ ${FLAGS} -o movecheck.o -c movecheck.cpp

.PHONY: clean

clean:
	rm -f *.o PieceBreaker

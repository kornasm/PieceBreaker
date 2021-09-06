FLAGS= -Wall -std=c++17 -Wshadow

all: main position functions generators move node
	g++ ${FLAGS} -o PieceBreaker main.o position.o functions.o generators.o move.o node.o

main: main.cpp
	g++ ${FLAGS} -o main.o -c main.cpp

position: position.cpp
	g++ ${FLAGS} -o position.o -c position.cpp

functions: functions.cpp
	g++ ${FLAGS} -o functions.o -c functions.cpp

generators: movegenerators.cpp
	g++ ${FLAGS} -o generators.o -c movegenerators.cpp

move: move.cpp
	g++ ${FLAGS} -o move.o -c move.cpp

node: node.cpp
	g++ ${FLAGS} -o node.o -c node.cpp

.PHONY: clean

clean:
	rm -f *.o PieceBreaker

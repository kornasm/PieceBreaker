FLAGS = -std=c++17 -Wall -Wshadow -Wextra -Wno-unused-parameter
OBJECTFILES = main.o position.o functions.o generators.o move.o node.o movecheck.o search.o evaluate.o uci.o

debug: FLAGS += -g
debug: all

release: FLAGS += -O2
release: all

all: ${OBJECTFILES}
	g++ ${FLAGS} -o PieceBreaker ${OBJECTFILES}

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

search.o: search.cpp search.h
	g++ ${FLAGS} -o search.o -c search.cpp

evaluate.o: evaluate.cpp evaluate.h
	g++ ${FLAGS} -o evaluate.o -c evaluate.cpp

uci.o: uci.cpp uci.h
	g++ ${FLAGS} -o uci.o -c uci.cpp

.PHONY: clean release debug

clean:
	rm -f *.o PieceBreaker

FLAGS = -std=c++17 -Wall -Wshadow -Wextra -Wno-unused-parameter -Wconversion
LIBS = -lpthread
OBJECTFILES = main.o position.o functions.o generators.o move.o node.o movecheck.o search.o evaluate.o uci.o

debug: FLAGS += -g
debug: all

release: FLAGS += -O2
release: all

all: ${OBJECTFILES}
	g++ ${FLAGS} -o PieceBreaker ${OBJECTFILES} ${LIBS}

main.o: main.cpp
	g++ ${FLAGS} -o main.o -c main.cpp ${LIBS}

position.o: position.cpp position.h
	g++ ${FLAGS} -o position.o -c position.cpp ${LIBS}

functions.o: functions.cpp functions.h
	g++ ${FLAGS} -o functions.o -c functions.cpp ${LIBS}

generators.o: movegenerators.cpp movegenerators.h
	g++ ${FLAGS} -o generators.o -c movegenerators.cpp ${LIBS}

move.o: move.cpp move.h
	g++ ${FLAGS} -o move.o -c move.cpp ${LIBS}

node.o: node.cpp node.h
	g++ ${FLAGS} -o node.o -c node.cpp ${LIBS}

movecheck.o: movecheck.cpp movecheck.h
	g++ ${FLAGS} -o movecheck.o -c movecheck.cpp ${LIBS}

search.o: search.cpp search.h
	g++ ${FLAGS} -o search.o -c search.cpp ${LIBS}

evaluate.o: evaluate.cpp evaluate.h
	g++ ${FLAGS} -o evaluate.o -c evaluate.cpp ${LIBS}

uci.o: uci.cpp uci.h
	g++ ${FLAGS} -o uci.o -c uci.cpp ${LIBS}

.PHONY: clean release debug

clean:
	rm -f *.o PieceBreaker

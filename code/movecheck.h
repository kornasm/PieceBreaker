#ifndef MOVECHECK_H_
#define MOVECHECK_H_

#include "declarations.h"
#include "functions.h"

class Position;
class Move;

class MoveChecker{
    public:
        virtual Move* CheckMoveLegality(Position *position, int from, int to){
            return NULL;
        }
        virtual ~MoveChecker(){};
};

class KingMoveChecker: public MoveChecker{
    public:
        Move* CheckMoveLegality(Position *position, int from, int to);
};

class RookMoveChecker: public MoveChecker{
    public:
        Move* CheckMoveLegality(Position *position, int from, int to);
};

class BishopMoveChecker: public MoveChecker{
    public:
        Move* CheckMoveLegality(Position *position, int from, int to);
};

class QueenMoveChecker: public MoveChecker{
    public:
        Move* CheckMoveLegality(Position *position, int from, int to);
};

class KnightMoveChecker: public MoveChecker{
    public:
        Move* CheckMoveLegality(Position *position, int from, int to);
};

class WhitePawnMoveChecker: public MoveChecker{
    public:
        Move* CheckMoveLegality(Position *position, int from, int to);
};

class BlackPawnMoveChecker: public MoveChecker{
    public:
        Move* CheckMoveLegality(Position *position, int from, int to);
};

class EmptyMoveChecker: public MoveChecker{
    public:
        Move* CheckMoveLegality(Position *position, int from, int to){
            return NULL;
        }
};

class MoveCheckHandler{
    static MoveChecker* checkers[NO_PIECES + 1];
    public:
        static void Init();
        static void cleanup();
        static Move* CheckMove(Position* position, int from, int to);
};

#endif
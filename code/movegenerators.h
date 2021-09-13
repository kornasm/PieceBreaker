#ifndef MOVEGEN_H_
#define MOVEGEN_H_

#include<list>
#include<iostream>
class Position;
class Move;

extern const int mailbox[64];

class MoveGenerator{
    public:
        virtual std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            return new std::list<Move>();
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position){
            return new std::list<Move>();
        }
        virtual ~MoveGenerator(){};
};

class KingMoveGenerator: public MoveGenerator{
    public:
        static const int KingPossibleSquares[8];
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class RookMoveGenerator: public MoveGenerator{
    public:
        static const int RookLeftSquares[7];
        static const int RookUpSquares[7];
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class BishopMoveGenerator: public MoveGenerator{
    public:
        static const int BishopUpRightSquares[7];
        static const int BishopUpLeftSquares[7];
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class KnightMoveGenerator: public MoveGenerator{
    public:
        static const int KnightSquares[8];
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class QueenMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class WhitePawnMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class BlackPawnMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class EmptyMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position) const{
            //std::cout << "EmptyMoveGenerator::GenerateMoveListStatic() executed (bad index typed)\n";
            return new std::list<Move>();
        }
};

class AllMovesGenerator{
    public:
        static std::list<Move>* GenerateMoves(Position *position, bool inverted = false);
};

void AddPromoMoves(std::list<Move>* list, Move baseMove, int color);

#endif
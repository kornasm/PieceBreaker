#ifndef MOVEGEN_H_
#define MOVEGEN_H_

#include<list>
#include<iostream>
class Position;
class Move;

class MoveGenerator{
    public:
        virtual std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            return new std::list<Move>();
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position){
            return new std::list<Move>();
        }
        virtual ~MoveGenerator(){};
};

class KingMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class RookMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class BishopMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class KnightMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class QueenMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class WhitePawnMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class BlackPawnMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            return GenerateMoveListStatic(originSquare, position);
        }
        static std::list<Move>* GenerateMoveListStatic(int originSquare, Position* position);
};

class EmptyMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListVirtual(int originSquare, Position* position){
            //std::cout << "EmptyMoveGenerator::GenerateMoveListStatic() executed (bad index typed)\n";
            return new std::list<Move>();
        }
};

class AllMovesGenerator{
    public:
        static std::list<Move>* GenerateMoves(Position *position);
};

#endif
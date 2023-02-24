#ifndef MOVEGEN_H_
#define MOVEGEN_H_

#include "declarations.h"

#include <list>
#include <iostream>

class Position;
class Move;
class Evaluator;

extern const int mailbox[64];

class MoveGenerator{
    public:
        virtual std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const{
            return new std::list<Move>();
        }
        virtual ~MoveGenerator(){};
};

class KingMoveGenerator: public MoveGenerator{
    private:
        static const int KingPossibleSquares[8];
        friend Evaluator;
    public:
        std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const;
};

class RookMoveGenerator: public MoveGenerator{
    public:
        static const int RookLeftSquares[7];
        static const int RookUpSquares[7];
    public:
        std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const;
};

class BishopMoveGenerator: public MoveGenerator{
    public:
        static const int BishopUpRightSquares[7];
        static const int BishopUpLeftSquares[7];
    public:
        std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const;
};

class KnightMoveGenerator: public MoveGenerator{
    public:
        static const int KnightSquares[8];
    public:
        std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const;
};

class QueenMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const;
};

class WhitePawnMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const;
};

class BlackPawnMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const;
};

class EmptyMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveList(int originSquare, const Position& position) const{
            return new std::list<Move>();
        }
};

class AllMovesGenerator{
    public:
        static std::list<Move>* GenerateMoves(const Position& position, bool inverted = false);
};

void AddPromoMoves(std::list<Move>* list, Move baseMove, int color);

class MoveGeneratorHandler{
    private:
        static MoveGenerator* generators[NO_PIECES + 1];
    public:
        static void Init();
        static void Cleanup();
        static std::list<Move>* GenerateMoves(int originSquare, const Position& position);
};

#endif
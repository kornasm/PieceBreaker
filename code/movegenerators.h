#include<list>
#include<iostream>
class Node;
class Move;

class MoveGenerator{
    public:
        virtual std::list<Move>* GenerateMoveListv(int position, Node* node){
            return new std::list<Move>();
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node){
            return new std::list<Move>();
        }
        virtual ~MoveGenerator(){};
};

class KingMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node);
};

class RookMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node);
};

class BishopMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node);
};

class KnightMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node);
};

class QueenMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node);
};

class WhitePawnMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node);
};

class BlackPawnMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node);
};

class EmptyMoveGenerator: public MoveGenerator{
    public:
        std::list<Move>* GenerateMoveListv(int position, Node* node){
            std::cout << "EmptyMoveGenerator::GenerateMoveList() executed (bad index typed)\n";
            return new std::list<Move>();
        }
        static std::list<Move>* GenerateMoveList(int position, Node* node){
            return new std::list<Move>();
        }
};
#include<list>
class Node;

class MoveGenerator{
    public:
        virtual std::list<int>* GenerateMoveListv(int position, Node* node){
            return new std::list<int>();
        }
        static std::list<int>* GenerateMoveList(int position, Node* node){
            return new std::list<int>();
        }

};

class KingMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node);
        static std::list<int>* GenerateMoveList(int position, Node* node);

};

class RookMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node);
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class BishopMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node);
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class KnightMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node);
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class QueenMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node);
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class WhitePawnMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node);
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class BlackPawnMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node);
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class EmptyMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node);
        static std::list<int>* GenerateMoveList(int position, Node* node);
};
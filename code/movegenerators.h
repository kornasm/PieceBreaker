#include<list>
class Node;

class MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class KingMoveGenerator: public MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class RookMoveGenerator: public MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class BishopMoveGenerator: public MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class KnightMoveGenerator: public MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class QueenMoveGenerator: public MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class WhitePawnMoveGenerator: public MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class BlackPawnMoveGenerator: public MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class EmptyMoveGenerator: public MoveGenerator{
    public:
        static std::list<int>* GenerateMoveList(int position, Node* node);
};
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
        virtual ~MoveGenerator(){};
};

class KingMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<int>* GenerateMoveList(int position, Node* node);
        //~KingMoveGenerator(){};
};

class RookMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<int>* GenerateMoveList(int position, Node* node);
        //~RookMoveGenerator(){};
};

class BishopMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<int>* GenerateMoveList(int position, Node* node);
        //~BishopMoveGenerator(){};
};

class KnightMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<int>* GenerateMoveList(int position, Node* node);
        //~KnightMoveGenerator(){};
};

class QueenMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<int>* GenerateMoveList(int position, Node* node);
        //~QueenMoveGenerator(){};
};

class WhitePawnMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class BlackPawnMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node){
            return GenerateMoveList(position, node);
        }
        static std::list<int>* GenerateMoveList(int position, Node* node);
};

class EmptyMoveGenerator: public MoveGenerator{
    public:
        std::list<int>* GenerateMoveListv(int position, Node* node){
            return new std::list<int>();
        }
        static std::list<int>* GenerateMoveList(int position, Node* node){
            return new std::list<int>();
        }
};
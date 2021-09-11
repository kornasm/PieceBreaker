#ifndef NODE_H_
#define NODE_H_

#include<list>
#include<string>

class Position;
class Move;

class Node{
    public:
        Position* position;
        std::list<Node*> children;

        int depth = 0;

        Node();
        Node(std::string fen);
        Node(Position *pos, Node* prev);
        ~Node();

        bool CheckMove(Move *move);
        void GenerateChildNodes();
};

#endif
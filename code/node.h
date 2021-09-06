#ifndef NODE_H_
#define NODE_H_

#include<list>

class Position;

class Node{
    public:
        Position* position;
        std::list<Node*> children;

        Node();
        Node(std::string fen);
        Node(Position *pos);
        ~Node();

        bool CheckMove(Move *move);
};

#endif
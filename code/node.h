#ifndef NODE_H_
#define NODE_H_

#include<list>
#include<string>
#include<stack>

class Position;
class Move;

class Node{
    public:
        Position *position;
        Move *moveMade = NULL;
        Node *prev = NULL;
        Node *bestmove = NULL;
        std::list<Node*> children;
        int value = 0; // position evaluation
        int bestval;

        int depth = 1;

    public:
        Node();
        Node(std::string fen);
        Node(Position *pos, Node* pr);
        void OnConstructing();
        ~Node();

        bool CheckMove(Move *move);
        void Search(std::stack<Node*>& stack, int maxDepth);
        void PassValueBackwards(Node *from);
        void Evaluate();
        int GetEval() const { return bestval; }

        friend std::ostream& operator <<(std::ostream& out, const Node& node);
};

std::ostream& operator <<(std::ostream& out, const Node& node);

#endif
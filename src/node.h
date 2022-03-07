#ifndef NODE_H_
#define NODE_H_

#include <list>
#include <string>

class Position;
class Move;

class Node{
    public:
        static int noNodes;
        static int noActiveNodes;

        Position *position;
        Move *moveMade = nullptr;
        Node *prev = nullptr;
        Node *bestmove = nullptr;
        std::list<Node*> children;
        float partialEval;
        int depth = 0;

    public:
        
        Node();
        Node(std::string fen);
        Node(Position *pos, Node* pr);
        void OnConstructing();
        ~Node();

        Node(const Node& nd) = delete;
        Node operator =(const Node& nd) = delete;

        bool CheckMove(Move *move);
        void Search(int maxDepth);
        void PassValueBackwards(Node *from);
        void Evaluate();
        
        double priority;
        double CalcPriority();

        // getters
        float GetEval() const { return partialEval; }
        int GetDepth() const { return depth; }
        long long GetHash() const;

        friend std::ostream& operator <<(std::ostream& out, const Node& node);
        friend void Explore(Node *nd, std::string prefix, int maxdepth);
        friend class SearchTree;
};

void Explore(Node *nd, std::string prefix = "", int maxdepth = 10);

std::ostream& operator <<(std::ostream& out, const Node& node);



#endif
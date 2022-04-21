#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <set>

class Position;
class Move;
class Node;

bool CompareNodesAscending(Node *nd1, Node *nd2);
bool CompareNodesDescending(Node* nd1, Node* nd2);

class Node{
    public:
        static int noNodes;
        static int noActiveNodes;

        Position *position;
        Move *moveMade = nullptr;
        Node *prev = nullptr;
        Node *bestmove = nullptr;
        std::set<Node*, decltype(&CompareNodesAscending)> children;
        float partialEval;
        int depth = 0;

    public:
        
        Node();
        Node(std::stringstream& strFen);
        Node(Position *pos, Node* pr);
        void OnConstructing();
        ~Node();

        Node(const Node& nd) = delete;
        Node operator =(const Node& nd) = delete;

        Node* CheckMove(Move *move);
        Node* MakeMove(Move *move, Position *newposition = nullptr);
        void Search(int maxDepth);
        void PassValueBackwards(Node *from);
        void Evaluate();

        void Isolate(); // prepare a root node to be searched
        
        double priority;
        double CalcPriority();

        // getters
        std::string GetFen();
        float GetEval() const { return partialEval; }
        int GetDepth() const { return depth; }
        long long GetHash() const;

        friend std::ostream& operator <<(std::ostream& out, const Node& node);
        friend void Explore(Node *nd, std::string prefix, int maxdepth, int outstream);
        friend class SearchTree;
};

void Explore(Node *nd, std::string prefix = "", int maxdepth = 10, int outstream = 0);

std::ostream& operator <<(std::ostream& out, const Node& node);



#endif
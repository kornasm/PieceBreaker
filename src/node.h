#ifndef NODE_H_
#define NODE_H_

#include "logger.h"

#include <string>
#include <set>

class Position;
class Move;
class Node;

bool CompareNodesAscending(Node *nd1, Node *nd2);
bool CompareNodesDescending(Node* nd1, Node* nd2);

class Node{
    private:
        static int noNodes;
        static int noActiveNodes;

        Position *position;
        Move *moveMade = nullptr;
        Node *prev = nullptr;
        Node *bestmove = nullptr;
        Node *bestmovePrevious = nullptr;
        std::set<Node*, decltype(&CompareNodesAscending)> children;
        float searchEval;
        int depth = 0;
        double priority;

    public:
        
        Node();
        Node(std::stringstream& strFen);
        Node(Position *pos, Node* pr);
        void OnConstructing();
        Node(const Node& nd) = delete;
        Node operator =(const Node& nd) = delete;

        ~Node();

        Node* CheckMove(Move *move);
        Node* MakeMove(Move *move, Position *newposition = nullptr);
        void Search(int maxDepth);
        void PassValueBackwards(Node *from);
        void Evaluate();
        void Isolate(); // prepare a root node to be searched
        double CalcPriority();

        // getters
        std::string GetFen() const;
        float GetEval() const { return searchEval; }
        int GetDepth() const { return depth; }
        long long GetHash() const;
        double GetPriority() const { return priority; }

        friend std::ostream& operator <<(std::ostream& out, const Node& node);
        friend void Explore(Node *nd, std::string prefix, int maxdepth, int outstream);
        friend class SearchTree;
};

void Explore(Node *nd, std::string prefix = "", int maxdepth = 10, int loge_level = LOG_ANALYSIS);

std::ostream& operator <<(std::ostream& out, const Node& node);

// evaluation results from GameResult class
// WHITE_WIN = 10000
// BLACK_WIN = -10000
// evaluation with absolute value over this value will mean checkmate was found during a search
#define CHECKMATE_LIMIT 9900

#endif
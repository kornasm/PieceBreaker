#ifndef NODE_H_
#define NODE_H_

#include<list>
#include<string>
#include<stack>
#include<memory>

class Position;
class Move;

class Node{
    public:
        Position *position;
        Move *moveMade = NULL;
        Node *prev = NULL;
        Node *bestmove = NULL;
        std::list<Node*> children;
        //int value = 0; // position evaluation
        int partialEval;
        int fullEval;
        int fullyEvaluated = 0; // 1 - some path leads to end, 2 - all paths lead to end
        int fullChildren = 0;
        int bestval;
        int searchedDepth = 0;

        int depth = 0;

    public:
        static int count;
        Node();
        Node(std::string fen);
        Node(Position *pos, Node* pr);
        void OnConstructing();
        ~Node();

        bool CheckMove(Move *move);
        void Search(int maxDepth);
        void PassValueBackwards(Node *from, int eval);
        void Evaluate();
        int GetEval() const { return partialEval; }

        friend std::ostream& operator <<(std::ostream& out, const Node& node);
};

/*class WhiteNode : public Node{

};

class BlackNode : public Node{

};//*/      
// bool operator >(std::unique_ptr<Node> n1, std::unique_ptr<Node> n2);

std::ostream& operator <<(std::ostream& out, const Node& node);

#endif
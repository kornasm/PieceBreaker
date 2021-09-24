#include "declarations.h"
#include "node.h"
#include "position.h"
#include "move.h"
#include "evaluate.h"

int Node::count = 0;

Node::Node(){
    position = new Position();
    prev = NULL;
    OnConstructing();
}

Node::Node(std::string fen){
    position = new Position(fen);
    prev = NULL;
    OnConstructing();
}

Node::Node(Position *pos, Node* pr){
    prev = pr;
    position = pos;
    if(prev){
        depth = prev->depth + 1;
    }
    prev->children.push_back(this);
    OnConstructing();
}

void Node::OnConstructing(){
    Evaluate();
    position->ToMove() == WHITE ? bestval = -999999 : bestval = 999999;
}

Node::~Node(){
    delete position;
    /*for(auto node : children){
        delete node;
    }//*/
    delete bestmove;
    delete moveMade;
    count++;
}

bool Node::CheckMove(Move *move){
    Position *newposition = position->MakeMove(move);
    if(newposition){
        children.push_back(new Node(newposition, this));
        return true;
    }
    return false;
}

void Node::Search(std::stack<Node*>& stack, int maxDepth){
    if(depth >= maxDepth || position->GetGameResult() != ONGOING){
        bestval = value;
        return;
    }
    //std::cout << *this;
    int best = 0;
    std::list<Move>* moves = position->GenerateAllLegalMoves();
    for(auto m : *moves){
        Move *move = new Move(m);
        Position *newpos = new Position(*position, move, move->Promo());
        Node *newnode = new Node(newpos, this);
        newnode->moveMade = move;
        newnode->Search(stack, maxDepth);
        if(position->ToMove() == WHITE){
            if(newnode->GetEval() > bestval){
                delete bestmove;
                bestmove = newnode;
                bestval = newnode->GetEval();
            }
            else{
                delete newnode;
            }
        }
        else{
            if(newnode->GetEval() < bestval){
                delete bestmove;
                bestmove = newnode;
                bestval = newnode->GetEval();
            }
            else{
                delete newnode;
            }
        }
    }
    delete moves;

    /*for(auto node : children){

    }//*/
}

void Node::PassValueBackwards(Node *from){
    std::cout << "passing back   ";
    if(!from) std::cout << "  from null";
    std::cout << '\n';

    int passedValue(0);
    //position->ToMove() == WHITE ? passedValue = 100000 : passedValue = -100000;
    if(from){
        passedValue = from->GetEval();
    }
    else{
        passedValue = value;
    }
    if(position->ToMove() == WHITE){
        if(passedValue < bestval){
            bestval = passedValue;
            bestmove = from;
            if(prev){
                prev->PassValueBackwards(this);
                return;
            }
        }
    }
    else{
        if(passedValue > bestval){
            bestval = passedValue;
            bestmove = from;
            if(prev){
                prev->PassValueBackwards(this);
                return;
            }
        }
    }
}

void Node::Evaluate(){
    value = Evaluator::Evaluate(*position);
}

std::ostream& operator <<(std::ostream& out, const Node& node){
    out << "--NODE--    depth   " << node.depth << "\n";
    node.position->ShowTinyBoard(out);
    out << "Eval:  " << node.value << '\n';
    return out;
}
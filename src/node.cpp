#include "node.h"

#include "declarations.h"
#include "position.h"
#include "move.h"
#include "evaluate.h"
#include "search.h"

#include <iomanip>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>

int Node::noNodes = 0;
int Node::noActiveNodes = 0;

Node::Node() :children(&CompareNodesDescending){
    position = new Position();
    prev = nullptr;
    moveMade = nullptr;
    OnConstructing();
}

Node::Node(std::stringstream& strFen) :children(&CompareNodesDescending){
    position = new Position(strFen);
    prev = nullptr;
    OnConstructing();
}

Node::Node(Position *pos, Node* pr) :children(&CompareNodesDescending){
    prev = pr;
    position = pos;
    if(prev){
        depth = prev->depth + 1;
    }
    OnConstructing();
    prev->children.insert(this);
}

void Node::OnConstructing(){
    Evaluate();
    noNodes++;
    noActiveNodes++;
    priority = CalcPriority();
}

Node::~Node(){
    delete position;
    for(auto nd = children.begin(); nd != children.end(); ++nd){
        delete *nd;
    }
    children.clear();
    if(moveMade){
        delete moveMade;
    }
    noActiveNodes--;
}

Node* Node::CheckMove(Move *move){
    Position *newposition = position->MakeMove(move);
    if(newposition){
        return MakeMove(move, newposition);
    }
    return nullptr;
}

Node* Node::MakeMove(Move *move, Position *newposition){
    if(newposition == nullptr){
        newposition = new Position(*position, move, move->Promo());
    }
    Node *newNode = new Node(newposition, this);
    newNode->moveMade = move;
    return newNode;
}

void Node::Search(int maxDepth){
    PassValueBackwards(nullptr);
    SearchTree* searchTree = SearchTree::GetInstance();
    if(depth >= maxDepth || position->GetGameResult() != ONGOING){
        return;
    }
    std::list<Move>* moves = position->GenerateAllLegalMoves();
    for(auto m : *moves){
        Move *move = new Move(m);
        MakeMove(move);
    }
    while(children.size() > 10){
        Node* toDelete = *(children.rbegin());
        children.erase(--children.end());
        delete toDelete;
    }
    for(auto nd = this->children.begin(); nd != this->children.end(); ++nd){
        searchTree->AddNodeToQueue(*nd);
    }
    delete moves;
}

void Node::PassValueBackwards(Node *from){
    if(from == nullptr){
        //std::cerr << "\npassing back   from\n" << *this;
        if(prev){
            prev->PassValueBackwards(this);
        }
        return;
    }
    //std::cerr << "passing back   to   depth  " << this->depth << '\n';

    bool changed = false;
    if(this->position->ToMove() == WHITE){
        if(from->partialEval > partialEval){
            changed = true;
            partialEval = from->partialEval;
            bestmove = from;
        }
        else{
            if(from == bestmove){
                float maxval = -1e6;
                Node* best = nullptr;
                //for(auto nd : this->children){
                for(auto nd = this->children.begin(); nd != this->children.end(); ++nd){
                    if((*nd)->partialEval > maxval){
                        maxval = (*nd)->partialEval;
                        best = *nd;
                    }
                }
                changed = true;
                this->partialEval = maxval;
                this->bestmove = best;
            }
        }
    }
    else{
        if(from->partialEval < partialEval){
            changed = true;
            partialEval = from->partialEval;
            bestmove = from;
        }
        else{
            if(from == bestmove){
                float maxval = 1e6;
                Node* best = nullptr;
                for(auto nd : this->children){
                    if(nd->partialEval < maxval){
                        maxval = nd->partialEval;
                        best = nd;
                    }
                }
                changed = true;
                this->partialEval = maxval;
                this->bestmove = best;
            }
        }
    }

    if(changed == true){
        if(prev){
            prev->PassValueBackwards(this);
        }
        if(depth == 0){
            //Explore(this, "", 1);
            std::cerr << "best path changed    from   " << *this->bestmove->moveMade << "   eval    " << this->bestmove->partialEval << '\n';
            Node *current = this->bestmove;
            std::cerr << bestmove << "\t" << partialEval << '\n';
            std::cerr << "moves  \n";
            while(current){
                std::cerr << *(current->moveMade) << '\n';
                current = current->bestmove;
            }
        }
    }
}

void Node::Evaluate(){
    partialEval = Evaluator::Evaluate(*position);
    //bestval = partialEval;
}

void Node::Isolate(){
    prev = nullptr;
    depth = 0;
}

double Node::CalcPriority(){
    double val = std::exp(-static_cast<double>(depth));
    double eval = -partialEval * static_cast<float>(position->ToMove());
    return val * eval;
}

std::string Node::GetFen(){
    return position->GetFen();
}

long long Node::GetHash() const { 
    return position->GetPositionHash(); 
}

std::ostream& operator <<(std::ostream& out, const Node& node){
    out << "--NODE--    depth   " << node.depth << std::endl;
    node.position->ShowTinyBoard(out);
        out << "Eval:  " << node.partialEval << std::endl;
   // }
    
    return out;
}

void Explore(Node *nd, std::string prefix, int maxdepth, int outstream){
    if(outstream == 0){
        std::cerr << prefix  << "NODE " << nd << "\tprev " << nd->prev << "\tdepth " << nd->depth << "\teval " << std::fixed << std::setprecision(1) << nd->partialEval << "\tbest " << nd->bestmove << "\t";
        if(nd->moveMade){
            std::cerr << *(nd->moveMade) << "   " << nd->moveMade->From() << "    " << nd->moveMade->To() << "   priority   " << nd->priority;
        }
        std::cerr << '\n';
        if(nd->GetDepth() < maxdepth){
            for(auto node = nd->children.begin(); node != nd->children.end(); ++node){
                Explore(*node, prefix + '\t', maxdepth, outstream);
            }
        }
        return;
    }
    std::cout << prefix  << "NODE " << nd << "\tprev " << nd->prev << "\tdepth " << nd->depth << "\teval " << std::fixed << std::setprecision(1) << nd->partialEval << "\tbest " << nd->bestmove << "\t";
    if(nd->moveMade){
        std::cout << *(nd->moveMade) << "   " << nd->moveMade->From() << "    " << nd->moveMade->To() << "   priority   " << nd->priority;
    }
    std::cout << '\n';
    if(nd->GetDepth() < maxdepth){
        for(auto node = nd->children.begin(); node != nd->children.end(); ++node){
            Explore(*node, prefix + '\t', maxdepth, outstream);
        }
    }
    return;
}


bool CompareNodesAscending(Node* nd1, Node* nd2){
    return nd1->priority < nd2->priority;
}

bool CompareNodesDescending(Node* nd1, Node* nd2){
    return nd1->priority >= nd2->priority;
}
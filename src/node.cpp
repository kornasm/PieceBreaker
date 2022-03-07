#include "node.h"

#include "declarations.h"
#include "position.h"
#include "move.h"
#include "evaluate.h"
#include "search.h"

#include <iomanip>
#include <iostream>
#include <cmath>

int Node::noNodes = 0;
int Node::noActiveNodes = 0;

Node::Node(){
    position = new Position();
    prev = nullptr;
    OnConstructing();

}

Node::Node(std::string fen){
    position = new Position(fen);
    prev = nullptr;
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
    noNodes++;
    noActiveNodes++;
    priority = CalcPriority();
}

Node::~Node(){
    delete position;
    for(auto node : children){ 
        delete node;
    }//*/
    children.clear();
    delete moveMade;
    noActiveNodes--;
}

bool Node::CheckMove(Move *move){
    Position *newposition = position->MakeMove(move);
    if(newposition){
        children.push_back(new Node(newposition, this));
        return true;
    }
    return false;
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
        Position *newpos = new Position(*position, move, move->Promo());
        Node *newnode = new Node(newpos, this);
        newnode->moveMade = move;
        //newnode->Search(maxDepth);
        searchTree->AddNodeToQueue(newnode);
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
                for(auto nd : this->children){
                    if(nd->partialEval > maxval){
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

double Node::CalcPriority(){
    double val = std::exp(-static_cast<double>(depth));
    double eval = -partialEval * static_cast<float>(position->ToMove());
    return val * eval;
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

void Explore(Node *nd, std::string prefix, int maxdepth){
    std::cerr << prefix  << "NODE " << nd << "\tprev " << nd->prev << "\tdepth " << nd->depth << "\teval " << std::fixed << std::setprecision(1) << nd->partialEval << "\tbest " << nd->bestmove << "\t";
    if(nd->moveMade){
        std::cerr << *(nd->moveMade) << "   " << nd->moveMade->From() << "    " << nd->moveMade->To() << "   priority   " << nd->priority;
    }
    std::cerr << '\n';
    if(nd->GetDepth() < maxdepth){
        for(auto node : nd->children){
            Explore(node, prefix + '\t', maxdepth);
        }
    }
    return;
}


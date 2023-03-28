#include "node.h"

#include "declarations.h"
#include "position.h"
#include "move.h"
#include "evaluate.h"
#include "search.h"
#include "logger.h"

#include <iomanip>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>

int Node::noNodes = 0;
int Node::noActiveNodes = 0;

extern Logger logger;

Node::Node()
   :position(new Position()),
    moveMade(nullptr),
    prev(nullptr),
    children(&CompareNodesDescending)
{
    OnConstructing();
}

Node::Node(std::stringstream& strFen)
   :position(new Position(strFen)),
    prev(nullptr),
    children(&CompareNodesDescending)
{
    OnConstructing();
}

Node::Node(Position *pos, Node* pr)
   :position(pos),
    prev(pr),
    children(&CompareNodesDescending)
{
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

void Node::Search(SearchTree *searchTree, int maxDepth){
    PassValueBackwards(nullptr);

    searchTree->IncreaseNodeCount();
    if(depth >= maxDepth || position->GetGameResult() != ONGOING){
        return;
    }
    std::list<Move> moves = position->GenerateAllLegalMoves();
    for(auto m : moves){
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
}

void Node::PassValueBackwards(Node *from){
    if(from == nullptr){
        logger << LogDest(LOG_DEBUG) << "\npassing back   from\n" << *this;
        if(prev){
            prev->PassValueBackwards(this);
        }
        return;
    }
    logger << LogDest(LOG_DEBUG) << "passing back   to   depth  " << this->depth << '\n';

    bool changed = false;

    if(bestmove == nullptr){
        changed = true;
        bestmove = from;
        searchEval = from->searchEval;
    }

    if(this->position->ToMove() == WHITE){
        if(from->searchEval > searchEval){
            changed = true;
            if(from->searchEval > CHECKMATE_LIMIT){
                logger << LogDest(LOG_ANALYSIS) << "checkmate passing back from  " << from << "\n";
                searchEval = from->searchEval - 1;
            }
            else{
                searchEval = from->searchEval;
            }
            bestmove = from;
        }
        else{
            if(from == bestmove){
                float maxval = -1e6;
                Node* best = nullptr;
                //for(auto nd : this->children){
                for(auto nd = this->children.begin(); nd != this->children.end(); ++nd){
                    if((*nd)->searchEval > maxval){
                        maxval = (*nd)->searchEval;
                        best = *nd;
                    }
                }
                changed = true;
                this->bestmovePrevious = this->bestmove;
                this->searchEval = maxval;
                this->bestmove = best;
            }
        }
    }
    else{
        if(from->searchEval < searchEval){
            changed = true;
            if(from->searchEval < -CHECKMATE_LIMIT){
                searchEval = from->searchEval - 1;
            }
            else{
                searchEval = from->searchEval;
            }
            bestmove = from;
        }
        else{
            if(from == bestmove){
                float maxval = 1e6;
                Node* best = nullptr;
                for(auto nd : this->children){
                    if(nd->searchEval < maxval){
                        maxval = nd->searchEval;
                        best = nd;
                    }
                }
                changed = true;
                this->bestmovePrevious = this->bestmove;
                this->searchEval = maxval;
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
            if(this->bestmovePrevious != this->bestmove){
                logger << LogDest(LOG_ANALYSIS) << "Best move chenged from  ";
                if(this->bestmovePrevious){
                    logger << *(this->bestmovePrevious->moveMade);
                }
                logger << "\n" <<                  "                    to  " << *this->bestmove->moveMade << "\n"; 
            }
            logger << "New eval          " << this->bestmove->searchEval << '\n';
            Node *current = this->bestmove;
            logger << bestmove << "\t" << searchEval << '\n';
            logger << "moves  \n";
            while(current){
                logger << *(current->moveMade) << '\n';
                current = current->bestmove;
            }
        }
    }
}

void Node::Evaluate(){
    Evaluator evaluator(this->position);
    searchEval = evaluator.Evaluate();
    //bestval = searchEval;
}

void Node::Isolate(){
    prev = nullptr;
    depth = 0;
}

double Node::CalcPriority(){
    double val = std::exp(-static_cast<double>(depth));
    double eval = -searchEval * static_cast<float>(position->ToMove());
    return val * eval;
}

std::string Node::GetFen() const{
    return position->GetFen();
}

long long Node::GetHash() const { 
    return position->GetPositionHash(); 
}

std::ostream& operator <<(std::ostream& out, const Node& node){
    out << "--NODE--    depth   " << node.depth << std::endl;
    node.position->ShowTinyBoard(out);
        out << "Eval:  " << node.searchEval << std::endl;
   // }
    
    return out;
}

void Explore(Node *nd, std::string prefix, int maxdepth, int log_level){

    logger << LogDest(log_level) << prefix  << "NODE " << nd << "\tprev " << nd->prev << "\tdepth " << nd->depth << "\teval " << std::fixed << nd->searchEval << "\tbest " << nd->bestmove << "\t";
    if(nd->moveMade){
        logger << LogDest(log_level) << *(nd->moveMade) << "   " << nd->moveMade->From() << "    " << nd->moveMade->To() << "   priority   " << nd->priority;
    }
    logger << LogDest(log_level) << '\n';
    if(nd->GetDepth() < maxdepth){
        for(auto node = nd->children.begin(); node != nd->children.end(); ++node){
            Explore(*node, prefix + '\t', maxdepth, log_level);
        }
    }
    return;
}


bool CompareNodesAscending(Node* nd1, Node* nd2){
    return nd1->GetPriority() < nd2->GetPriority();
}

bool CompareNodesDescending(Node* nd1, Node* nd2){
    return nd1->GetPriority() >= nd2->GetPriority();
}
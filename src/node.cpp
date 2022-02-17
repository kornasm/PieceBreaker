#include "declarations.h"
#include "node.h"
#include "position.h"
#include "move.h"
#include "evaluate.h"
#include "search.h"

#include <iomanip>

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
    //position->ToMove() == WHITE ? bestval = -999999 : bestval = 999999;
}

Node::~Node(){
    /*std::cerr << "DESTRUCTOR\n";
    std::cerr << prev << "   " << position << "   " << moveMade << '\n';
    std::cerr << *this << '\n';//*/
    //prev = NULL;
    delete position;
    for(auto node : children){ 
        //std::cerr << this << "   deletes    " << node << std::endl;
        delete node;
    }//*/
    children.clear();
    //delete bestmove;
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

void Node::Search(int maxDepth){
    PassValueBackwards(NULL);
    //char waitchar;
    //std::cin >> waitchar;
    SearchTree* searchTree = SearchTree::GetInstance();
    if(depth >= maxDepth || position->GetGameResult() != ONGOING){
        bestval = partialEval;
        return;
    }
    //std::cout << *this;
    //int best = 0;
    std::list<Move>* moves = position->GenerateAllLegalMoves();
    for(auto m : *moves){
        Move *move = new Move(m);
        Position *newpos = new Position(*position, move, move->Promo());
        Node *newnode = new Node(newpos, this);
        newnode->moveMade = move;
        //newnode->Search(maxDepth);
        searchTree->AddNodeToQueue(newnode);
        //children.push_back(newnode);
        /*if(position->ToMove() == WHITE){
            if(newnode->GetEval() > bestval){
                delete bestmove;
                bestmove = newnode; 
                bestval = newnode->GetEval();
                //PassValueBackwards(NULL);
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
        }//*/
    }
    delete moves;
}

void Node::PassValueBackwards(Node *from){
    if(from == NULL){
        //std::cerr << "\npassing back   from\n" << *this;
        //char waitchar;
        //std::cin >> waitchar;
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
            Explore(this, "", 1);
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

    
    /*if(position->ToMove() == WHITE){
        if(passedValue > bestval){
            bestval = passedValue;
            bestmove = from;
            if(prev){
                prev->PassValueBackwards(this, bestval);
                return;
            } 
            else{
                std::cout << "new best move";
            }
        }
    }
    else{
        if(passedValue < bestval){
            bestval = passedValue;
            bestmove = from;
            if(prev){
                prev->PassValueBackwards(this, bestval);
                return;
            }
            else{
                std::cout << "new best move";
            }
        }
    }//*/
}

void Node::Evaluate(){
    partialEval = Evaluator::Evaluate(*position);
    bestval = partialEval;
    //GameResult res = position->GetGameResult();
    /*if(res != GameResult::ONGOING){
        fullyEvaluated = 2;
        fullEval = partialEval;
    }//*/
    
}

std::ostream& operator <<(std::ostream& out, const Node& node){
    out << "--NODE--    depth   " << node.depth << std::endl;
    node.position->ShowTinyBoard(out);
    //if(node.fullyEvaluated == true){
    //    out << "Eval full:  " << node.fullEval << std::endl;     
   // }
   // else{
        out << "Eval partial:  " << node.partialEval << std::endl;
   // }
    
    return out;
}

void Explore(Node *nd, std::string prefix, int maxdepth){
    std::cerr << prefix  << "NODE " << nd << "\tprev " << nd->prev << "\tdepth " << nd->depth << "\teval " << std::fixed << std::setprecision(1) << nd->partialEval << "\tbest " << nd->bestmove << "\t";
    if(nd->moveMade){
        std::cerr << *(nd->moveMade) << "   " << nd->moveMade->From() << "    " << nd->moveMade->To();
    }
    std::cerr << '\n';
    if(nd->GetDepth() < maxdepth){
        for(auto node : nd->children){
            Explore(node, prefix + '\t', maxdepth);
        }
    }
    return;
}
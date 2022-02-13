#include "declarations.h"
#include "node.h"
#include "position.h"
#include "move.h"
#include "evaluate.h"
#include "search.h"

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
    std::cerr << "DESTRUCTOR\n";
    std::cerr << prev << "   " << position << "   " << moveMade << '\n';
    std::cerr << *this << '\n';
    //prev = NULL;
    delete position;
    for(auto node : children){ 
        std::cerr << this << "   deletes    " << node << std::endl;
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
    PassValueBackwards(NULL, 0);
    char waitchar;
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

void Node::PassValueBackwards(Node *from, int eval){
    if(from == NULL){
        std::cerr << "\npassing back   from\n" << *this;
        char waitchar;
        //std::cin >> waitchar;
        if(prev){
            prev->PassValueBackwards(this, bestval);
        }
        return;
    }
    std::cerr << "passing back   to   depth" << this->depth << '\n';

    int passedValue = from->GetEval();

    /*if(from->fullyEvaluated > this->fullyEvaluated){
        if(from->fullyEvaluated == 2){

        }
    }
    if(from->searchedDepth > searchedDepth){
        searchedDepth = from->searchedDepth;
        bestval = from->GetEval();
    }//*/
    int maxval = 0;
    Node* best = nullptr;
    bool changed = false;
    if(this->position->ToMove() == WHITE){
        maxval = -1e6;
        for(auto nd : this->children){
            if(nd->partialEval > maxval){
                maxval = nd->partialEval;
                best = nd;
            }
        }
        if(this->partialEval != maxval){
            changed = true;
        }
        this->partialEval = maxval;
        this->bestmove = best;
    }
    else{
        maxval = 1e6;
        for(auto nd : this->children){
            if(nd->partialEval < maxval){
                maxval = nd->partialEval;
                best = nd;
            }
        }
        if(this->partialEval != maxval){
            changed = true;
        }
        this->partialEval = maxval;
        this->bestmove = best;
    }

    if(changed == true){
        if(prev){
            prev->PassValueBackwards(this, this->partialEval);
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
    GameResult res = position->GetGameResult();
    if(res != GameResult::ONGOING){
        fullyEvaluated = 2;
        fullEval = partialEval;
    }
    
}

std::ostream& operator <<(std::ostream& out, const Node& node){
    out << "--NODE--    depth   " << node.depth << std::endl;
    node.position->ShowTinyBoard(out);
    if(node.fullyEvaluated == true){
        out << "Eval full:  " << node.fullEval << std::endl;     
    }
    else{
        out << "Eval partial:  " << node.partialEval << std::endl;
    }
    
    return out;
}
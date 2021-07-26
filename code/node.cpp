#include "declarations.h"

extern MoveGenerator* generators[NO_PIECES + 1];
Node::Node(){
    _board = new Board();
}

Node::~Node(){
    delete _board;
}
void Node::ShowBoard(){
    _board->ShowBoard();
}

Board* Node::GetBoardPtr(){
    return _board;
}

bool Node::MakeMove(std::string notation){
    int from = Not2Ind(notation.substr(0, 2));
    int to = Not2Ind(notation.substr(2, 2));
    if(from > 120){
        std::cout << "index out of range\n";
        return false;
    }
    std::list<int>* moves = generators[_board->GetSquareValue(from) + SYMBOLS_OFFSET]->GenerateMoveListv(from, this);
    PrintMoveList(moves);
    bool available = false;
    auto it = moves->begin();
    while(it != moves->end()){
        if(to == *it){
            available = true;
            break;
        }
        ++it;
    }
    if(available){
        _board->MakeMove(from, to);
    }
    std::cout << '\n';
    delete moves;
    return true;
}
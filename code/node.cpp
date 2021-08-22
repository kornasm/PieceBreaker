#include "declarations.h"

extern MoveGenerator* generators[NO_PIECES + 1];
Node::Node(){
    _board = new Board(this);
    _toMove = WHITE;
    _whcstl = 3;
    _blcstl = 3;
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

bool Node::CheckMove(std::string notation){
    int from = Not2Ind(notation.substr(0, 2));
    int to = Not2Ind(notation.substr(2, 2));
    if(from < 0 || from > 120){
        std::cout << "index out of range\n";
        return false;
    }
    int PieceColor = _board->GetSquareColor(from);
    if(_toMove != PieceColor){
        std::cout << "wrong piece chosen (wrong color)\n";
        return false;
    }
    if(_board->CheckMove(from, to))
    {
        _toMove *= (-1);
        return true;
    }
    return false;
}
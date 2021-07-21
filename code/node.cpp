#include "declarations.h"

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
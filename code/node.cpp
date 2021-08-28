#include "declarations.h"

extern MoveGenerator* generators[NO_PIECES + 1];
Node::Node(){
    _board = new Board(this);
    _toMove = WHITE;
    _whcstl = 3;
    _blcstl = 3;
    _enPassant = -1;
    prev = NULL;
}

Node::Node(Node *pr, Board* b, Move *m){
    prev = pr;
    _board = b;
    b->parentnode = this;
    _toMove = prev->_toMove * (-1);
    _whcstl = prev->_whcstl;
    _blcstl = prev->_blcstl;
    if(pr->_toMove == WHITE){
        if(_whcstl % SHORT_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e1") || m->From() == Not2Ind("h1")){
                _whcstl -= SHORT_CASTLE_MOVE / 2;
            }
        }
        if(_whcstl % LONG_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e1") || m->From() == Not2Ind("a1")){
                _whcstl -= LONG_CASTLE_MOVE / 2;
            }
        }
    }
    else{
        if(_blcstl % SHORT_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e8") || m->From() == Not2Ind("h8")){
                _blcstl -= SHORT_CASTLE_MOVE / 2;
            }
        }
        if(_blcstl % LONG_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e8") || m->From() == Not2Ind("a8")){
                _blcstl -= LONG_CASTLE_MOVE / 2;
            }
        }
    }
    if(m->Type() == PAWN_DOUBLE_MOVE){
        _enPassant = (m->From() + m->To()) / 2;
    }
    else{
        _enPassant = -1;
    }//*/
    if(m->From() == b->_whiteKingPos){
        b->_whiteKingPos = m->To();
    }
    if(m->From() == b->_blackKingPos){
        b->_blackKingPos = m->To();
    }
}

Node::~Node(){
    //std::cout << "node  " << this << "  destr   " << _board << '\n';
    delete _board;
    for(auto nd : children){
        delete nd;   
    }//*/
    //std::cout << "node  " << this << "  destr\n"; 
}
void Node::ShowBoard(){
    _board->ShowBoard();
}

Board* Node::GetBoardPtr(){
    return _board;
}


bool Node::CheckMove(Move *checkedmove, bool execute){
    int PieceColor = _board->GetSquareColor(checkedmove->From());
    if(_toMove != PieceColor){
        std::cout << "wrong piece chosen (wrong color)\n";
        return false;
    }
    Move *expectedmove = _board->CheckMove(checkedmove->From(), checkedmove->To());
    if(NULL != expectedmove)
    {
        //std::cout << "expected move found\n\n";
        if(((expectedmove->Type() & PROMOTION_MOVE) == PROMOTION_MOVE) && checkedmove->Promo() == EMPTY_SQUARE){
            std::cout << "No promo char entered\n";
            delete expectedmove;
            return false;
        } 
        //std::cout << "creating new board\n";
        Board *newboard = new Board(_board, expectedmove, checkedmove->Promo());
        Node *newnode = new Node(this, newboard, expectedmove);
        if(_toMove == WHITE){
            if(newboard->IsPlaceAttacked(newboard->_whiteKingPos, newnode->_toMove)){
                std::cout << "king attacked after the move\n";
                delete newnode;
                delete expectedmove;
                return false;
            }
        }
        else{
            if(newboard->IsPlaceAttacked(newboard->_blackKingPos, newnode->_toMove)){
                std::cout << "king attacked after the move\n";
                delete newnode;
                delete expectedmove;
                return false;
            }
        }
        if(execute == true){
            children.push_back(newnode);
        }
        else{
            delete newnode;
        }
        delete expectedmove;
        return true;
    }
    return false;
}

void Node::CheckCheck(){
    if(_toMove == WHITE){
        if(_board->IsPlaceAttacked(_board->_whiteKingPos, -_toMove)){
            underCheck = true;
        }
        else{
            underCheck = false;
        }
    }
    else{
        if(_board->IsPlaceAttacked(_board->_blackKingPos, -_toMove)){
            underCheck = true;
        }
        else{
            underCheck = false;
        }
    }
}

std::list<Move>* Node::GenerateAllLegalMoves(){
    std::list<Move>* moves = AllMovesGenerator::GenerateMoves(this);
    std::list<Move>* result = new std::list<Move>();
    auto it = moves->begin();
    while(it != moves->end()){
        if(CheckMove(&(*it), false)){
            result->push_back(*it);
        }
        ++it;
    }
    delete moves;
    return result;
}
#include "declarations.h"

char PromotionSymbols[4] = {'R', 'N', 'B', 'Q'};
int  PromotionAnswers[4] = { 2 ,  3 ,  4 ,  5 };

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
}

Node::~Node(){
    delete _board;
    for(auto nd : children){
        delete nd;   
    }//*/
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
    int promo(EMPTY_SQUARE);
    if(notation.length() == 5){
        char promosymbol = notation[4];
        for(int i = 0; i < 4; i++){
            if(promosymbol == PromotionSymbols[i]){
                promo = PromotionAnswers[i];
            }
        }
    }
    std::cout << promo << "\n";
    if(from < 0 || from > 120){
        std::cout << "index out of range\n";
        return false;
    }
    int PieceColor = _board->GetSquareColor(from);
    if(_toMove != PieceColor){
        std::cout << "wrong piece chosen (wrong color)\n";
        return false;
    }
    Move *expectedmove = _board->CheckMove(from, to);
    if(NULL != expectedmove)
    {
        std::cout << "expected move found\n\n";
        if(((expectedmove->Type() & PROMOTION_MOVE) == PROMOTION_MOVE) && promo == EMPTY_SQUARE){
            std::cout << "No promo char entered\n";
            delete expectedmove;
            return false;
        } 
        std::cout << "creating new board\n";
        Board *newboard = new Board(_board, expectedmove, promo);
        Node *newnode = new Node(this, newboard, expectedmove);
        std::cout << "new board created\n";
        if(newboard->IsPlaceAttacked(newboard->_whiteKingPos, newnode->_toMove)){
            std::cout << "king attacked after the move\n";
            delete newnode;
            delete expectedmove;
            return false;
        }
        else{
            std::cout << "inserting node to list\n";
            //children.push_back(Node(this, newboard, expectedmove));
            //children.push_back(std::make_shared<Node>(this, newboard, expectedmove));
            children.push_back(newnode);
            std::cout << "inserted\n";
            //newboard->ShowBoard();
            std::cout << "board shown\n";
        }
        //*/
        delete expectedmove;
        return true;
    }
    return false;
}

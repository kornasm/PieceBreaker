#include "declarations.h"

extern int mailbox[64];
extern MoveGenerator* generators[NO_PIECES + 1];
/*Board::Board(){
    _whiteKingPos = 26;
    _blackKingPos = 96;
};//*/

Board::Board(Node* nd){
    _whiteKingPos = Not2Ind("e1");
    _blackKingPos = Not2Ind("e8");
    parentnode = nd;
}

Board::Board(Board* b, Move *m, int promo){
    parentnode = NULL;
    std::copy(b->_squares, b->_squares + 122, _squares);
    _whiteKingPos = b->_whiteKingPos;
    _blackKingPos = b->_blackKingPos;

    _squares[m->To()] = _squares[m->From()];
    _squares[m->From()] = EMPTY_SQUARE;
    if(m->Type() == EN_PASSANT_MOVE){
        if(column(m->From()) < column(m->To())){
            _squares[m->From() + 1] = 0;
        }
        else{
            _squares[m->From() - 1] = 0;
        }
    }
    if(m->Type() == SHORT_CASTLE_MOVE){
        _squares[m->To() - 1] = _squares[m->To() + 1];
        _squares[m->To() + 1] = EMPTY_SQUARE;
    }
    if(m->Type() == LONG_CASTLE_MOVE){
        _squares[m->To() + 1] = _squares[m->To() - 2];
        _squares[m->To() - 2] = EMPTY_SQUARE;
    }
    if(m->Type() == PROMOTION_MOVE){
        std::cout << "promo move update symbol" << std::endl;
        if(_squares[m->To() < 0]){
            _squares[m->To()] = -promo;
        }
        else{
            _squares[m->To()] = promo;
        }
    }
}

Board::~Board(){
    //std::cout << "   board   " << this << "  destr   " << parentnode << '\n';
    parentnode = NULL;
}

void Board::ShowBoard(){
    std::cout << std::endl;
    std::cout << "  +---+---+---+---+---+---+---+---+\n";
    for(int row = 8; row > 0; row--){
        std::cout << row << " ";
        for(int col = 1; col <= 8; col++){
            std::cout << "| " << GetPiece(col, row) << " ";
        }
        std::cout << "|";
        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "    a   b   c   d   e   f   g   h\n\n";
    return;
}

char Board::GetPiece(int column, int row){
    return GetPieceSymbol(GetSquareValue(column, row));
}

int Board::GetSquareValue(int column, int row){
    if(column < 1 || column > 8 || row < 1 || row > 8){
        std::cout << "Wrong argument passed to Node::GetPiece:  column = " << column << "   row = " << row << std::endl;
    }
    return _squares[11 + 10 * row + column];
}
int Board::GetSquareColor(int column, int row){
    int val = GetSquareValue(column, row);
    if(val == OUTSIDE_BOARD){
        return OUTSIDE_BOARD;
    }
    else{
        return sgn(val);
    }
}

char Board::GetPiece(int index){
    return GetPieceSymbol(_squares[index]);
}
int Board::GetSquareValue(int index){
    return _squares[index];
}
int Board::GetSquareColor(int index){
    int val = GetSquareValue(index);
    if(val == OUTSIDE_BOARD){
        return OUTSIDE_BOARD;
    }
    else{
        return sgn(_squares[index]);
    }
}

Move* Board::CheckMove(int from, int to){
    std::list<Move>* moves = generators[this->GetSquareValue(from) + SYMBOLS_OFFSET]->GenerateMoveListVirtual(from, parentnode);

    Move *expected = new Move(from, to, REGULAR_MOVE);
    bool available = false;
    for(auto m : *moves){
        if(m == *expected){ 
            available = true;
            *expected = m;
            break;
        }
    }
    delete moves;
    if(available){
        return expected;
    }
    delete expected;
    return NULL;
}

bool Board::IsPlaceAttacked(int attackedplace, int attackingcolor){
    bool answer = false;
    //std::cout << "is king attacked:    " << attackedplace << "  " << attackingcolor << '\n';
    for(int i = 0; i < 64 && answer == false; i++){
        int ind = mailbox[i];
        if(GetSquareColor(ind) == attackingcolor){
            std::list<Move>* moves = generators[_squares[ind] + SYMBOLS_OFFSET]->GenerateMoveListVirtual(ind, parentnode);
            auto it = moves->begin();
            while(it != moves->end()){
                if(it->To() == attackedplace){
                    answer = true;
                }
                ++it;
            }
            delete moves;
        }
    }
    //std::cout << "is place attacked end\n\n";
    return answer;
}
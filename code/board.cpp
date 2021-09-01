#include "declarations.h"

extern int mailbox[64];
extern MoveGenerator* generators[NO_PIECES + 1];

Board::Board(Node* nd){
    whiteKingPos = Not2Ind("e1");
    blackKingPos = Not2Ind("e8");
    parentnode = nd;
}

Board::Board(Board* b, Move *m, int promo){
    parentnode = NULL;
    std::copy(b->squares, b->squares + 122, squares);
    whiteKingPos = b->whiteKingPos;
    blackKingPos = b->blackKingPos;

    squares[m->To()] = squares[m->From()];
    squares[m->From()] = EMPTY_SQUARE;
    if(m->Type() == EN_PASSANT_MOVE){
        if(column(m->From()) < column(m->To())){
            squares[m->From() + 1] = 0;
        }
        else{
            squares[m->From() - 1] = 0;
        }
    }
    if(m->Type() == SHORT_CASTLE_MOVE){
        squares[m->To() - 1] = squares[m->To() + 1];
        squares[m->To() + 1] = EMPTY_SQUARE;
    }
    if(m->Type() == LONG_CASTLE_MOVE){
        squares[m->To() + 1] = squares[m->To() - 2];
        squares[m->To() - 2] = EMPTY_SQUARE;
    }
    if(m->Type() == PROMOTION_MOVE){
        std::cout << "promo move update symbol" << std::endl;
        if(squares[m->To() < 0]){
            squares[m->To()] = -promo;
        }
        else{
            squares[m->To()] = promo;
        }
    }
}

Board::~Board(){
    parentnode = NULL;
}

void Board::ShowBoard() const{
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

char Board::GetPiece(int column, int row) const{
    return GetPieceSymbol(GetSquareValue(column, row));
}

int Board::GetSquareValue(int column, int row) const{
    if(column < 1 || column > 8 || row < 1 || row > 8){
        std::cout << "Wrong argument passed to Node::GetPiece:  column = " << column << "   row = " << row << std::endl;
    }
    return squares[11 + 10 * row + column];
}
int Board::GetSquareColor(int column, int row) const{
    int val = GetSquareValue(column, row);
    if(val == OUTSIDE_BOARD){
        return OUTSIDE_BOARD;
    }
    else{
        return sgn(val);
    }
}

char Board::GetPiece(int index) const{
    return GetPieceSymbol(squares[index]);
}
int Board::GetSquareValue(int index) const{
    return squares[index];
}
int Board::GetSquareColor(int index) const{
    int val = GetSquareValue(index);
    if(val == OUTSIDE_BOARD){
        return OUTSIDE_BOARD;
    }
    else{
        return sgn(squares[index]);
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
            std::list<Move>* moves = generators[squares[ind] + SYMBOLS_OFFSET]->GenerateMoveListVirtual(ind, parentnode);
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
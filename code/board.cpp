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

bool Board::CheckMove(int from, int to){
    std::list<Move>* moves = generators[this->GetSquareValue(from) + SYMBOLS_OFFSET]->GenerateMoveListv(from, parentnode);
    PrintMoveList(moves);
    std::cout << Move::count << '\n';
    std::cout << "expected before\n";
    Move* expected = new Move(from, to, REGULAR_MOVE);
    std::cout << "expected after\n";
    bool available = false;
    //auto it = moves->begin();
    std::cout << "itit\n\n";
    /*while(it != moves->end()){
        std::cout << "loop\n";
        if(*expected == *it){
            std::cout << *it;
            std::cout << "ifif\n";
            available = true;
            break;
        }
        ++it;
    }//*/
    for(auto m : *moves){
        std::cout << "loop\n";
        if(m == *expected){
            available = true;
            break;
        }
    }
    std::cout << "deleting move list\n\n";
    delete moves;
    std::cout << "deleted move list\n";
    delete expected;
    std::cout << "deleted expected move\n\n";
    if(available){
        _squares[to] = _squares[from];
        _squares[from] = 0;
        if(_whiteKingPos == from){
            _whiteKingPos = to;
        }
        if(_blackKingPos == from){
            _blackKingPos = to;
        }
        return true;
    }
    return false;
}

bool Board::IsPlaceAttacked(int attackedplace, int attackingcolor){
    bool answer = false;
    for(int i = 0; i < 64 && answer == false; i++){
        int ind = mailbox[i];
        if(GetSquareColor(ind) == attackingcolor){
            std::list<Move>* moves = generators[_squares[ind]]->GenerateMoveList(ind, parentnode);
            auto it = moves->begin();
            while(it != moves->end()){
                if(it->To() == attackedplace){
                    answer = true;
                }
            }
            delete moves;
        }
    }
    return answer;
}
#include "declarations.h"

Board::Board(){
    std::cout << "Board const\n";
}

void Board::ShowBoard(){
    std::cout << "showboard" << std::endl;
    for(int row = 8; row > 0; row--){
        for(int col = 1; col <= 8; col++){
            std::cout << GetPiece(col, row) << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
    for(int row = 8; row > 0; row--){
        for(int col = 1; col <= 8; col++){
            std::cout << GetSquareColor(col, row) << " ";
        }
        std::cout << '\n';
    }
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
    return sgn(_squares[index]);
}
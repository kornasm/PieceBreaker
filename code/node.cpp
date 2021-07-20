#include "declarations.h"

Node::Node(){}

void Node::ShowBoard(){
    std::cout << "showboard" << std::endl;
    for(int row = 8; row > 0; row--){
        for(int col = 1; col <= 8; col++){
            std::cout << GetPiece(col, row) << " ";
        }
        std::cout << '\n';
    }
    return;
}
char Node::GetPiece(int column, int row){
    if(column < 1 || column > 8 || row < 1 || row > 8){
        std::cout << "Wrong argument passed to Node::GetPiece:  column = " << column << "   row = " << row << std::endl;
    }
    return GetPieceSymbol(_board[11 + 10 * row + column]);
}
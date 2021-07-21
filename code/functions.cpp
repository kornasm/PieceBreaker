#include "declarations.h"

int KingPossibleSquares[8] = {-11, -10, -9, -1, 1, 9, 10, 11};

char GetPieceSymbol(int piece_number){
    return PiecesSymbols[piece_number + SYMBOLS_OFFSET];
    //return '\0';
}

std::list<int>* GenerateKingMoves(int position, Node *node){
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    for(auto move : KingPossibleSquares){
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)){
            moves->push_back(position + move);
        }
    }
    return moves;
}
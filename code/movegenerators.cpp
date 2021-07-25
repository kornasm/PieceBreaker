#include "declarations.h"

int KingPossibleSquares[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
int RookLeftSquares[7] = {-1, -2, -3, -4, -5, -6, -7};
int RookUpSquares[7] = {10, 20, 30, 40, 50, 60, 70};
int BishopUpRightSquares[7] = {11, 22, 33, 44, 55, 66, 77};
int BishopUpLeftSquares[7] = {9, 18, 27, 36, 45, 54, 63};
int KnightSquares[8] = {-21, -19, -8, 12, 21, 19, 8, -12};

std::list<int>* KingMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    for(auto move : KingPossibleSquares){
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)){
            moves->push_back(position + move);
        }
    }
    return moves;
}

std::list<int>* RookMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    for(auto move : RookLeftSquares){
        if(board->GetSquareValue(position + move != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)))
            moves->push_back(position + move);
        else
            break;
    }
    for(auto move : RookUpSquares){
        if(board->GetSquareValue(position + move != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)))
            moves->push_back(position + move);
        else
            break;
    }
    for(auto move : RookLeftSquares){
        if(board->GetSquareValue(position - move != OUTSIDE_BOARD && board->GetSquareColor(position - move) != board->GetSquareColor(position)))
            moves->push_back(position - move);
        else
            break;
    }
    for(auto move : RookUpSquares){
        if(board->GetSquareValue(position - move != OUTSIDE_BOARD && board->GetSquareColor(position - move) != board->GetSquareColor(position)))
            moves->push_back(position - move);
        else
            break;
    }
    return moves;
}

std::list<int>* BishopMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    for(auto move : BishopUpRightSquares){
        if(board->GetSquareValue(position + move != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)))
            moves->push_back(position + move);
        else
            break;
    }
    for(auto move : BishopUpLeftSquares){
        if(board->GetSquareValue(position + move != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)))
            moves->push_back(position + move);
        else
            break;
    }
    for(auto move : BishopUpRightSquares){
        if(board->GetSquareValue(position - move != OUTSIDE_BOARD && board->GetSquareColor(position - move) != board->GetSquareColor(position)))
            moves->push_back(position - move);
        else
            break;
    }
    for(auto move : BishopUpLeftSquares){
        if(board->GetSquareValue(position - move != OUTSIDE_BOARD && board->GetSquareColor(position - move) != board->GetSquareColor(position)))
            moves->push_back(position - move);
        else
            break;
    }
    return moves;
}

std::list<int>* QueenMoveGenerator::GenerateMoveList(int position, Node* node){
    std::list<int>* rookmoves = RookMoveGenerator::GenerateMoveList(position, node);
    std::list<int>* bishmoves = BishopMoveGenerator::GenerateMoveList(position, node);
    rookmoves->splice(rookmoves->end(), *bishmoves);
    return rookmoves;
}

std::list<int>* KnightMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    for(auto move : KnightSquares){
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)){
            moves->push_back(position + move);
        }
    }
    return moves;
}

std::list<int>* WhitePawnMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    if(board->GetSquareColor(position + 10) == EMPTY_SQUARE){
        moves->push_back(position + 10);
        if(row(position) == 2 && board->GetSquareColor(position + 20) == EMPTY_SQUARE){
            moves->push_back(position + 20);
        }
    }
    if(board->GetSquareColor(position) == -(board->GetSquareColor(position + 9))){
        moves->push_back(position + 9);
    }
    if(board->GetSquareColor(position) == -(board->GetSquareColor(position + 11))){
        moves->push_back(position + 11);
    }
    return moves;
}

std::list<int>* BlackPawnMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    if(board->GetSquareColor(position - 10) == EMPTY_SQUARE){
        moves->push_back(position - 10);
        if(row(position) == 7 && board->GetSquareColor(position - 20) == EMPTY_SQUARE){
            moves->push_back(position - 20);
        }
    }
    if(board->GetSquareColor(position) == -(board->GetSquareColor(position - 9))){
        moves->push_back(position - 9);
    }
    if(board->GetSquareColor(position) == -(board->GetSquareColor(position - 11))){
        moves->push_back(position - 11);
    }
    return moves;
}

#include "declarations.h"

int KingPossibleSquares[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
int RookLeftSquares[7] = {-1, -2, -3, -4, -5, -6, -7};
int RookUpSquares[7] = {10, 20, 30, 40, 50, 60, 70};
int BishopUpRightSquares[7] = {11, 22, 33, 44, 55, 66, 77};
int BishopUpLeftSquares[7] = {9, 18, 27, 36, 45, 54, 63};
int KnightSquares[8] = {-21, -19, -8, 12, 21, 19, 8, -12};

std::list<int>* KingMoveGenerator::GenerateMoveListv(int position, Node* node){
    return KingMoveGenerator::GenerateMoveList(position, node);
}
std::list<int>* KingMoveGenerator::GenerateMoveList(int position, Node* node){
    std::cout << "king move generator   generate moves\n";
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    for(auto move : KingPossibleSquares){
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)){
            moves->push_back(position + move);
        }
    }
    return moves;
}

std::list<int>* RookMoveGenerator::GenerateMoveListv(int position, Node* node){
    return RookMoveGenerator::GenerateMoveList(position, node);
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
std::list<int>* BishopMoveGenerator::GenerateMoveListv(int position, Node* node){
    return BishopMoveGenerator::GenerateMoveList(position, node);
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

std::list<int>* QueenMoveGenerator::GenerateMoveListv(int position, Node* node){
    return QueenMoveGenerator::GenerateMoveList(position, node);
}
std::list<int>* QueenMoveGenerator::GenerateMoveList(int position, Node* node){
    std::list<int>* rookmoves = RookMoveGenerator::GenerateMoveList(position, node);
    std::list<int>* bishmoves = BishopMoveGenerator::GenerateMoveList(position, node);
    rookmoves->splice(rookmoves->end(), *bishmoves);
    return rookmoves;
}

std::list<int>* KnightMoveGenerator::GenerateMoveListv(int position, Node* node){
    return KnightMoveGenerator::GenerateMoveList(position, node);
}
std::list<int>* KnightMoveGenerator::GenerateMoveList(int position, Node* node){
    std::cout << "knight move generator   generate moves\n";
    Board* board = node->GetBoardPtr();
    std::list<int>* moves = new std::list<int>();
    for(auto move : KnightSquares){
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)){
            moves->push_back(position + move);
        }
    }
    return moves;
}

std::list<int>* WhitePawnMoveGenerator::GenerateMoveListv(int position, Node* node){
    return WhitePawnMoveGenerator::GenerateMoveList(position, node);
}
std::list<int>* WhitePawnMoveGenerator::GenerateMoveList(int position, Node* node){
    return NULL;
}

std::list<int>* BlackPawnMoveGenerator::GenerateMoveListv(int position, Node* node){
    return BlackPawnMoveGenerator::GenerateMoveList(position, node);
}
std::list<int>* BlackPawnMoveGenerator::GenerateMoveList(int position, Node* node){
    return NULL;
}

std::list<int>* EmptyMoveGenerator::GenerateMoveListv(int position, Node* node){
    return new std::list<int>();
}
std::list<int>* EmptyMoveGenerator::GenerateMoveList(int position, Node* node){
    return new std::list<int>();
}
#include "declarations.h"

int KingPossibleSquares[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
int RookLeftSquares[7] = {-1, -2, -3, -4, -5, -6, -7};
int RookUpSquares[7] = {10, 20, 30, 40, 50, 60, 70};
int BishopUpRightSquares[7] = {11, 22, 33, 44, 55, 66, 77};
int BishopUpLeftSquares[7] = {9, 18, 27, 36, 45, 54, 63};
int KnightSquares[8] = {-21, -19, -8, 12, 21, 19, 8, -12};

std::list<Move>* KingMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : KingPossibleSquares){
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)){
            //moves->push_back(position + move);
            moves->push_back(Move::CreateMove(position, position + move, board->GetSquareColor(position + move) != EMPTY_SQUARE));
        }
    }
    return moves;
}

std::list<Move>* RookMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : RookLeftSquares){
        int TargetColor = board->GetSquareColor(position + move);
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && TargetColor != board->GetSquareColor(position)){
            moves->push_back(Move::CreateMove(position, position + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookUpSquares){
        int TargetColor = board->GetSquareColor(position + move);
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && TargetColor != board->GetSquareColor(position)){
            moves->push_back(Move::CreateMove(position, position + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookLeftSquares){
        int TargetColor = board->GetSquareColor(position - move);
        if(board->GetSquareValue(position - move) != OUTSIDE_BOARD && TargetColor != board->GetSquareColor(position)){
            moves->push_back(Move::CreateMove(position, position - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookUpSquares){
        int TargetColor = board->GetSquareColor(position - move);
        if(board->GetSquareValue(position - move) != OUTSIDE_BOARD && TargetColor != board->GetSquareColor(position)){
            moves->push_back(Move::CreateMove(position, position - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    return moves;
}

std::list<Move>* BishopMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : BishopUpRightSquares){
        int TargetColor = board->GetSquareColor(position + move);
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && TargetColor != board->GetSquareColor(position)){
            moves->push_back(Move::CreateMove(position, position + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpLeftSquares){
        int TargetColor = board->GetSquareColor(position + move);
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && TargetColor != board->GetSquareColor(position)){
            moves->push_back(Move::CreateMove(position, position + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpRightSquares){
        int TargetColor = board->GetSquareColor(position - move);
        if(board->GetSquareValue(position - move) != OUTSIDE_BOARD && TargetColor != board->GetSquareColor(position)){
            moves->push_back(Move::CreateMove(position, position - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpLeftSquares){
        int TargetColor = board->GetSquareColor(position - move);
        if(board->GetSquareValue(position - move) != OUTSIDE_BOARD && TargetColor != board->GetSquareColor(position)){
            moves->push_back(Move::CreateMove(position, position - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    return moves;
}

std::list<Move>* QueenMoveGenerator::GenerateMoveList(int position, Node* node){
    std::list<Move>* rookmoves = RookMoveGenerator::GenerateMoveList(position, node);
    std::list<Move>* bishmoves = BishopMoveGenerator::GenerateMoveList(position, node);
    rookmoves->splice(rookmoves->end(), *bishmoves);
    return rookmoves;
}

std::list<Move>* KnightMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : KnightSquares){
        if(board->GetSquareValue(position + move) != OUTSIDE_BOARD && board->GetSquareColor(position + move) != board->GetSquareColor(position)){
            //moves->push_back(position + move);
            moves->push_back(Move::CreateMove(position, position + move, board->GetSquareColor(position + move) != EMPTY_SQUARE));
        }
    }
    return moves;
}

std::list<Move>* WhitePawnMoveGenerator::GenerateMoveList(int position, Node* node){
    std::cout << "white pawn gen\n";
    Board* board = node->GetBoardPtr();
    std::list<Move>* moves = new std::list<Move>();
    if(board->GetSquareColor(position + 10) == EMPTY_SQUARE){
        //moves->push_back(position + 10);
        moves->push_back(Move::CreateMove(position, position + 10, board->GetSquareColor(position + 10) != EMPTY_SQUARE));
        if(row(position) == 2 && board->GetSquareColor(position + 20) == EMPTY_SQUARE){
            //moves->push_back(position + 20);
            moves->push_back(Move::CreateMove(position, position + 20, board->GetSquareColor(position + 20) != EMPTY_SQUARE));
        }
    }
    if(board->GetSquareColor(position) == -(board->GetSquareColor(position + 9))){
        //moves->push_back(position + 9);
        moves->push_back(Move::CreateMove(position, position + 9, CAPTURE_MOVE));
    }
    if(board->GetSquareColor(position) == -(board->GetSquareColor(position + 11))){
        //moves->push_back(position + 11);
        moves->push_back(Move::CreateMove(position, position +11, CAPTURE_MOVE));
    }
    return moves;
}

std::list<Move>* BlackPawnMoveGenerator::GenerateMoveList(int position, Node* node){
    Board* board = node->GetBoardPtr();
    std::list<Move>* moves = new std::list<Move>();
    if(board->GetSquareColor(position - 10) == EMPTY_SQUARE){
        //moves->push_back(position - 10);
        moves->push_back(Move::CreateMove(position, position - 10, board->GetSquareColor(position - 10) != EMPTY_SQUARE));
        if(row(position) == 7 && board->GetSquareColor(position - 20) == EMPTY_SQUARE){
            //moves->push_back(position - 20);
            moves->push_back(Move::CreateMove(position, position - 20, board->GetSquareColor(position - 20) != EMPTY_SQUARE));
        }
    }
    if(board->GetSquareColor(position) == -(board->GetSquareColor(position - 9))){
        //moves->push_back(position - 9);
        moves->push_back(Move::CreateMove(position, position - 9, CAPTURE_MOVE));
    }
    if(board->GetSquareColor(position) == -(board->GetSquareColor(position - 11))){
        //moves->push_back(position - 11);
        moves->push_back(Move::CreateMove(position, position - 11, CAPTURE_MOVE));
    }
    return moves;
}

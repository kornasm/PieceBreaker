#include "declarations.h"

int KingPossibleSquares[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
int RookLeftSquares[7] = {-1, -2, -3, -4, -5, -6, -7};
int RookUpSquares[7] = {10, 20, 30, 40, 50, 60, 70};
int BishopUpRightSquares[7] = {11, 22, 33, 44, 55, 66, 77};
int BishopUpLeftSquares[7] = {9, 18, 27, 36, 45, 54, 63};
int KnightSquares[8] = {-21, -19, -8, 12, 21, 19, 8, -12};
extern int mailbox[64];
extern MoveGenerator* generators[NO_PIECES + 1];
extern char PiecesSymbols[NO_PIECES];

std::list<Move>* KingMoveGenerator::GenerateMoveListStatic(int position, Node* node){
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : KingPossibleSquares){
        if(node->GetSquareValue(position + move) != OUTSIDE_BOARD && 
           node->GetSquareColor(position + move) != node->GetSquareColor(position)){
            moves->push_back(Move(position, position + move, node->GetSquareColor(position + move) != EMPTY_SQUARE));
        }
    }
    int col = node->GetSquareColor(position);
    if(col == WHITE){
        if((node->whcstl & SHORT_CASTLE_MOVE) != 0){
            if(node->GetSquareColor(position + 1) == EMPTY_SQUARE && node->GetSquareColor(position + 2) == EMPTY_SQUARE){
                //std::cout << Ind2Not(position + 1) << "  " << PiecesSymbols[node->GetSquareValue(position + 1) + SYMBOLS_OFFSET] << '\n';
                if(node->IsPlaceAttacked(position, -col) == false && 
                   node->IsPlaceAttacked(position + 1, -col) == false && 
                   node->IsPlaceAttacked(position + 2, -col) == false){
                    moves->push_back(Move(position, position + 2, SHORT_CASTLE_MOVE));
                }
            }
        }
        if((node->whcstl & LONG_CASTLE_MOVE) != 0){
            if(node->GetSquareColor(position - 1) == EMPTY_SQUARE && node->GetSquareColor(position - 2) == EMPTY_SQUARE){
                if(node->IsPlaceAttacked(position, -col) == false && 
                   node->IsPlaceAttacked(position - 1, -col) == false && 
                   node->IsPlaceAttacked(position - 2, -col) == false){
                    moves->push_back(Move(position, position - 2, LONG_CASTLE_MOVE));
                }
            }
        }
    }
    if(col == BLACK){
        if((node->blcstl & SHORT_CASTLE_MOVE) != 0){
            if(node->GetSquareColor(position + 1) == EMPTY_SQUARE && node->GetSquareColor(position + 2) == EMPTY_SQUARE){
                if(node->IsPlaceAttacked(position, -col) == false && 
                   node->IsPlaceAttacked(position + 1, -col) == false && 
                   node->IsPlaceAttacked(position + 2, -col) == false){
                    moves->push_back(Move(position, position + 2, SHORT_CASTLE_MOVE));
                }
            }
        }
        if((node->blcstl & LONG_CASTLE_MOVE) != 0){
            if(node->GetSquareColor(position - 1) == EMPTY_SQUARE && node->GetSquareColor(position - 2) == EMPTY_SQUARE){
                if(node->IsPlaceAttacked(position, -col) == false && 
                   node->IsPlaceAttacked(position - 1, -col) == false && 
                   node->IsPlaceAttacked(position - 2, -col) == false){
                    moves->push_back(Move(position, position - 2, LONG_CASTLE_MOVE));
                }
            }
        }
    }
    return moves;
}

std::list<Move>* RookMoveGenerator::GenerateMoveListStatic(int position, Node* node){
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : RookLeftSquares){
        int TargetColor = node->GetSquareColor(position + move);
        if(node->GetSquareValue(position + move) != OUTSIDE_BOARD && TargetColor != node->GetSquareColor(position)){
            moves->push_back(Move(position, position + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookUpSquares){
        int TargetColor = node->GetSquareColor(position + move);
        if(node->GetSquareValue(position + move) != OUTSIDE_BOARD && TargetColor != node->GetSquareColor(position)){
            moves->push_back(Move(position, position + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookLeftSquares){
        int TargetColor = node->GetSquareColor(position - move);
        if(node->GetSquareValue(position - move) != OUTSIDE_BOARD && TargetColor != node->GetSquareColor(position)){
            moves->push_back(Move(position, position - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookUpSquares){
        int TargetColor = node->GetSquareColor(position - move);
        if(node->GetSquareValue(position - move) != OUTSIDE_BOARD && TargetColor != node->GetSquareColor(position)){
            moves->push_back(Move(position, position - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    return moves; 
}

std::list<Move>* BishopMoveGenerator::GenerateMoveListStatic(int position, Node* node){
    std::list<Move> *moves = new std::list<Move>();
    for(auto move : BishopUpRightSquares){
        int TargetColor = node->GetSquareColor(position + move);
        if(node->GetSquareValue(position + move) != OUTSIDE_BOARD && TargetColor != node->GetSquareColor(position)){
            moves->push_back(Move(position, position + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpLeftSquares){
        int TargetColor = node->GetSquareColor(position + move);
        if(node->GetSquareValue(position + move) != OUTSIDE_BOARD && TargetColor != node->GetSquareColor(position)){
            moves->push_back(Move(position, position + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpRightSquares){
        int TargetColor = node->GetSquareColor(position - move);
        if(node->GetSquareValue(position - move) != OUTSIDE_BOARD && TargetColor != node->GetSquareColor(position)){
            moves->push_back(Move(position, position - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpLeftSquares){
        int TargetColor = node->GetSquareColor(position - move);
        if(node->GetSquareValue(position - move) != OUTSIDE_BOARD && TargetColor != node->GetSquareColor(position)){
            moves->push_back(Move(position, position - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    return moves;
}

std::list<Move>* QueenMoveGenerator::GenerateMoveListStatic(int position, Node* node){
    std::list<Move>* rookmoves = RookMoveGenerator::GenerateMoveListStatic(position, node);
    std::list<Move>* bishmoves = BishopMoveGenerator::GenerateMoveListStatic(position, node);
    rookmoves->splice(rookmoves->end(), *bishmoves);
    delete bishmoves;
    return rookmoves;
}

std::list<Move>* KnightMoveGenerator::GenerateMoveListStatic(int position, Node* node){
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : KnightSquares){
        if(node->GetSquareValue(position + move) != OUTSIDE_BOARD && 
           node->GetSquareColor(position + move) != node->GetSquareColor(position)){
            moves->push_back(Move(position, position + move, node->GetSquareColor(position + move) != EMPTY_SQUARE));
        }
    }
    return moves;
}

std::list<Move>* WhitePawnMoveGenerator::GenerateMoveListStatic(int position, Node* node){
    std::list<Move>* moves = new std::list<Move>();
    if(node->GetSquareColor(position + 10) == EMPTY_SQUARE){
        Move m(position, position + 10, PAWN_MOVE | (node->GetSquareColor(position + 10) != EMPTY_SQUARE));
        if(row(position) == 7){
            m.IncreaseType(PROMOTION_MOVE);
        }
        moves->push_back(m);
        
        if(row(position) == 2 && node->GetSquareColor(position + 20) == EMPTY_SQUARE){
            moves->push_back(Move(position, position + 20, PAWN_MOVE | PAWN_DOUBLE_MOVE));
        }
    }
    if(node->GetSquareColor(position) == -(node->GetSquareColor(position + 9))){
        Move m(position, position + 9, PAWN_MOVE | CAPTURE_MOVE);
        if(row(position) == 7){
            m.IncreaseType(PROMOTION_MOVE);
        }
        moves->push_back(m);
        
    }
    if(node->enPassant == (position + 9) && node->GetSquareColor(position + 9) == EMPTY_SQUARE){
        moves->push_back(Move(position, position + 9, PAWN_MOVE | EN_PASSANT_MOVE));
    }
    if(node->GetSquareColor(position) == -(node->GetSquareColor(position + 11))){
        Move m(position, position + 11, PAWN_MOVE | CAPTURE_MOVE);
        if(row(position) == 7){
            m.IncreaseType(PROMOTION_MOVE);
        }
        moves->push_back(m);
        
    }
    if(node->enPassant == (position + 11) && node->GetSquareColor(position + 11) == EMPTY_SQUARE){
        moves->push_back(Move(position, position + 11, PAWN_MOVE | EN_PASSANT_MOVE));
    }
    return moves;
}

std::list<Move>* BlackPawnMoveGenerator::GenerateMoveListStatic(int position, Node* node){
    std::list<Move>* moves = new std::list<Move>();
    if(node->GetSquareColor(position - 10) == EMPTY_SQUARE){
        Move m(position, position - 10, PAWN_MOVE | (node->GetSquareColor(position - 10) != EMPTY_SQUARE));
        if(row(position) == 2){
            m.IncreaseType(PROMOTION_MOVE);
        }
        moves->push_back(m);
        if(row(position) == 7 && node->GetSquareColor(position - 20) == EMPTY_SQUARE){
            moves->push_back(Move(position, position - 20, PAWN_MOVE | PAWN_DOUBLE_MOVE));
        }
        
    }
    if(node->GetSquareColor(position) == -(node->GetSquareColor(position - 9))){
        Move m(position, position - 9, PAWN_MOVE | CAPTURE_MOVE);
        if(row(position) == 2){
            m.IncreaseType(PROMOTION_MOVE);
        }
        moves->push_back(m);   
    }
    if(node->enPassant == (position - 9) && node->GetSquareColor(position - 9) == EMPTY_SQUARE){
        moves->push_back(Move(position, position - 9, PAWN_MOVE | EN_PASSANT_MOVE));
    }
    if(node->GetSquareColor(position) == -(node->GetSquareColor(position - 11))){
        Move m(position, position - 11, PAWN_MOVE | CAPTURE_MOVE);
        if(row(position) == 2){
            m.IncreaseType(PROMOTION_MOVE);
        }
        moves->push_back(m);
    }
    if(node->enPassant == (position - 11) && node->GetSquareColor(position - 11) == EMPTY_SQUARE){
        moves->push_back(Move(position, position - 11, PAWN_MOVE | EN_PASSANT_MOVE));
    }
    return moves;
}

std::list<Move>* AllMovesGenerator::GenerateMoves(Node *node){
    std::list<Move> *moves = new std::list<Move>();
    for(int i = 0; i < 64; i++){
        int ind = mailbox[i];
        if(node->GetSquareColor(ind) == node->toMove){
            std::list<Move> *m = generators[node->GetSquareValue(ind) + SYMBOLS_OFFSET]->GenerateMoveListVirtual(ind, node);
            moves->splice(moves->end(), *m);
            delete m;
        }
    }
    return moves;
}
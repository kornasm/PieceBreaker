#include "movegenerators.h"
#include "declarations.h"
#include "position.h"
#include "move.h"
#include "functions.h"
#include <array>

/*MoveGenerator* gens[NO_PIECES + 1] = {new KingMoveGenerator(),new QueenMoveGenerator(),
                                              new BishopMoveGenerator(),new KnightMoveGenerator(),
                                              new RookMoveGenerator(),new BlackPawnMoveGenerator(),
                                              new EmptyMoveGenerator(),new WhitePawnMoveGenerator(),
                                              new RookMoveGenerator(),new KnightMoveGenerator(),
                                              new BishopMoveGenerator(),new QueenMoveGenerator(),
                                              new KingMoveGenerator(),new EmptyMoveGenerator()};//*/

MoveGenerator* generators[NO_PIECES + 1];

const int KingMoveGenerator::KingPossibleSquares[] = {-11, -10, -9, -1, 1, 9, 10, 11};
const int RookMoveGenerator::RookLeftSquares[] = {-1, -2, -3, -4, -5, -6, -7};
const int RookMoveGenerator::RookUpSquares[] = {10, 20, 30, 40, 50, 60, 70};
const int BishopMoveGenerator::BishopUpRightSquares[] = {11, 22, 33, 44, 55, 66, 77};
const int BishopMoveGenerator::BishopUpLeftSquares[] = {9, 18, 27, 36, 45, 54, 63};
const int KnightMoveGenerator::KnightSquares[] = {-21, -19, -8, 12, 21, 19, 8, -12};

std::list<Move>* KingMoveGenerator::GenerateMoveListStatic(int originSquare, Position* position){
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : KingPossibleSquares){
        if(position->GetSquareValue(originSquare + move) != OUTSIDE_BOARD && 
           position->GetSquareColor(originSquare + move) != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare + move, position->GetSquareColor(originSquare + move) != EMPTY_SQUARE));
        }
    }
    int col = position->GetSquareColor(originSquare);
    if(col == WHITE){
        if((position->whcstl & SHORT_CASTLE_MOVE) != 0){
            if(position->GetSquareColor(originSquare + 1) == EMPTY_SQUARE && position->GetSquareColor(originSquare + 2) == EMPTY_SQUARE){
                //std::cout << Ind2Not(originSquare + 1) << "  " << PiecesSymbols[position->GetSquareValue(originSquare + 1) + SYMBOLS_OFFSET] << '\n';
                if(position->IsPlaceAttacked(originSquare, -col) == false && 
                   position->IsPlaceAttacked(originSquare + 1, -col) == false && 
                   position->IsPlaceAttacked(originSquare + 2, -col) == false){
                    moves->push_back(Move(originSquare, originSquare + 2, SHORT_CASTLE_MOVE));
                }
            }
        }
        if((position->whcstl & LONG_CASTLE_MOVE) != 0){
            if(position->GetSquareColor(originSquare - 1) == EMPTY_SQUARE && position->GetSquareColor(originSquare - 2) == EMPTY_SQUARE){
                if(position->IsPlaceAttacked(originSquare, -col) == false && 
                   position->IsPlaceAttacked(originSquare - 1, -col) == false && 
                   position->IsPlaceAttacked(originSquare - 2, -col) == false){
                    moves->push_back(Move(originSquare, originSquare - 2, LONG_CASTLE_MOVE));
                }
            }
        }
    }
    if(col == BLACK){
        if((position->blcstl & SHORT_CASTLE_MOVE) != 0){
            if(position->GetSquareColor(originSquare + 1) == EMPTY_SQUARE && position->GetSquareColor(originSquare + 2) == EMPTY_SQUARE){
                if(position->IsPlaceAttacked(originSquare, -col) == false && 
                   position->IsPlaceAttacked(originSquare + 1, -col) == false && 
                   position->IsPlaceAttacked(originSquare + 2, -col) == false){
                    moves->push_back(Move(originSquare, originSquare + 2, SHORT_CASTLE_MOVE));
                }
            }
        }
        if((position->blcstl & LONG_CASTLE_MOVE) != 0){
            if(position->GetSquareColor(originSquare - 1) == EMPTY_SQUARE && position->GetSquareColor(originSquare - 2) == EMPTY_SQUARE){
                if(position->IsPlaceAttacked(originSquare, -col) == false && 
                   position->IsPlaceAttacked(originSquare - 1, -col) == false && 
                   position->IsPlaceAttacked(originSquare - 2, -col) == false){
                    moves->push_back(Move(originSquare, originSquare - 2, LONG_CASTLE_MOVE));
                }
            }
        }
    }
    return moves;
}

std::list<Move>* RookMoveGenerator::GenerateMoveListStatic(int originSquare, Position* position){
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : RookLeftSquares){
        int TargetColor = position->GetSquareColor(originSquare + move);
        if(position->GetSquareValue(originSquare + move) != OUTSIDE_BOARD && TargetColor != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookUpSquares){
        int TargetColor = position->GetSquareColor(originSquare + move);
        if(position->GetSquareValue(originSquare + move) != OUTSIDE_BOARD && TargetColor != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookLeftSquares){
        int TargetColor = position->GetSquareColor(originSquare - move);
        if(position->GetSquareValue(originSquare - move) != OUTSIDE_BOARD && TargetColor != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : RookUpSquares){
        int TargetColor = position->GetSquareColor(originSquare - move);
        if(position->GetSquareValue(originSquare - move) != OUTSIDE_BOARD && TargetColor != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    return moves; 
}

std::list<Move>* BishopMoveGenerator::GenerateMoveListStatic(int originSquare, Position* position){
    std::list<Move> *moves = new std::list<Move>();
    for(auto move : BishopUpRightSquares){
        int TargetColor = position->GetSquareColor(originSquare + move);
        if(position->GetSquareValue(originSquare + move) != OUTSIDE_BOARD && TargetColor != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpLeftSquares){
        int TargetColor = position->GetSquareColor(originSquare + move);
        if(position->GetSquareValue(originSquare + move) != OUTSIDE_BOARD && TargetColor != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare + move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpRightSquares){
        int TargetColor = position->GetSquareColor(originSquare - move);
        if(position->GetSquareValue(originSquare - move) != OUTSIDE_BOARD && TargetColor != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    for(auto move : BishopUpLeftSquares){
        int TargetColor = position->GetSquareColor(originSquare - move);
        if(position->GetSquareValue(originSquare - move) != OUTSIDE_BOARD && TargetColor != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare - move, TargetColor != EMPTY_SQUARE));
            if(TargetColor != EMPTY_SQUARE)
                break;
        }
        else
            break;
    }
    return moves;
}

std::list<Move>* QueenMoveGenerator::GenerateMoveListStatic(int originSquare, Position* position){
    std::list<Move>* rookmoves = RookMoveGenerator::GenerateMoveListStatic(originSquare, position);
    std::list<Move>* bishmoves = BishopMoveGenerator::GenerateMoveListStatic(originSquare, position);
    rookmoves->splice(rookmoves->end(), *bishmoves);
    delete bishmoves;
    return rookmoves;
}

std::list<Move>* KnightMoveGenerator::GenerateMoveListStatic(int originSquare, Position* position){
    std::list<Move>* moves = new std::list<Move>();
    for(auto move : KnightSquares){
        if(position->GetSquareValue(originSquare + move) != OUTSIDE_BOARD && 
           position->GetSquareColor(originSquare + move) != position->GetSquareColor(originSquare)){
            moves->push_back(Move(originSquare, originSquare + move, position->GetSquareColor(originSquare + move) != EMPTY_SQUARE));
        }
    }
    return moves;
}

std::list<Move>* WhitePawnMoveGenerator::GenerateMoveListStatic(int originSquare, Position* position){
    std::list<Move>* moves = new std::list<Move>();
    if(position->GetSquareColor(originSquare + 10) == EMPTY_SQUARE){
        Move m(originSquare, originSquare + 10, PAWN_MOVE | (position->GetSquareColor(originSquare + 10) != EMPTY_SQUARE));
        if(row(originSquare) == 7){
            m.IncreaseType(PROMOTION_MOVE);
            AddPromoMoves(moves, m, WHITE);
        }
        else{
            moves->push_back(m);
        }
        if(row(originSquare) == 2 && position->GetSquareColor(originSquare + 20) == EMPTY_SQUARE){
            moves->push_back(Move(originSquare, originSquare + 20, PAWN_MOVE | PAWN_DOUBLE_MOVE));
        }
    }
    if(position->GetSquareColor(originSquare) == -(position->GetSquareColor(originSquare + 9))){
        Move m(originSquare, originSquare + 9, PAWN_MOVE | CAPTURE_MOVE);
        if(row(originSquare) == 7){
            m.IncreaseType(PROMOTION_MOVE);
            AddPromoMoves(moves, m, WHITE);
        }
        else{
            moves->push_back(m);
        }
    }
    if(position->enPassant == (originSquare + 9) && position->GetSquareColor(originSquare + 9) == EMPTY_SQUARE){
        moves->push_back(Move(originSquare, originSquare + 9, PAWN_MOVE | EN_PASSANT_MOVE));
    }
    if(position->GetSquareColor(originSquare) == -(position->GetSquareColor(originSquare + 11))){
        Move m(originSquare, originSquare + 11, PAWN_MOVE | CAPTURE_MOVE);
        if(row(originSquare) == 7){
            m.IncreaseType(PROMOTION_MOVE);
            AddPromoMoves(moves, m, WHITE);
        }
        else{
            moves->push_back(m);
        }
    }
    if(position->enPassant == (originSquare + 11) && position->GetSquareColor(originSquare + 11) == EMPTY_SQUARE){
        moves->push_back(Move(originSquare, originSquare + 11, PAWN_MOVE | EN_PASSANT_MOVE));
    }
    return moves;
}

std::list<Move>* BlackPawnMoveGenerator::GenerateMoveListStatic(int originSquare, Position* position){
    std::list<Move>* moves = new std::list<Move>();
    if(position->GetSquareColor(originSquare - 10) == EMPTY_SQUARE){
        Move m(originSquare, originSquare - 10, PAWN_MOVE | (position->GetSquareColor(originSquare - 10) != EMPTY_SQUARE));
        if(row(originSquare) == 2){
            m.IncreaseType(PROMOTION_MOVE);
            AddPromoMoves(moves, m, BLACK);
        }
        else{
            moves->push_back(m);
        }
        if(row(originSquare) == 7 && position->GetSquareColor(originSquare - 20) == EMPTY_SQUARE){
            moves->push_back(Move(originSquare, originSquare - 20, PAWN_MOVE | PAWN_DOUBLE_MOVE));
        }
        
    }
    if(position->GetSquareColor(originSquare) == -(position->GetSquareColor(originSquare - 9))){
        Move m(originSquare, originSquare - 9, PAWN_MOVE | CAPTURE_MOVE);
        if(row(originSquare) == 2){
            m.IncreaseType(PROMOTION_MOVE);
            AddPromoMoves(moves, m, BLACK);
        }
        else{
            moves->push_back(m);
        }  
    }
    if(position->enPassant == (originSquare - 9) && position->GetSquareColor(originSquare - 9) == EMPTY_SQUARE){
        moves->push_back(Move(originSquare, originSquare - 9, PAWN_MOVE | EN_PASSANT_MOVE));
    }
    if(position->GetSquareColor(originSquare) == -(position->GetSquareColor(originSquare - 11))){
        Move m(originSquare, originSquare - 11, PAWN_MOVE | CAPTURE_MOVE);
        if(row(originSquare) == 2){
            m.IncreaseType(PROMOTION_MOVE);
            AddPromoMoves(moves, m, BLACK);
        }
        else{
            moves->push_back(m);
        }
    }
    if(position->enPassant == (originSquare - 11) && position->GetSquareColor(originSquare - 11) == EMPTY_SQUARE){
        moves->push_back(Move(originSquare, originSquare - 11, PAWN_MOVE | EN_PASSANT_MOVE));
    }
    return moves;
}

std::list<Move>* AllMovesGenerator::GenerateMoves(Position *position, bool inverted){
    std::list<Move> *moves = new std::list<Move>();
    for(int i = 0; i < 64; i++){
        int ind = mailbox[i];
        int movingColor = position->toMove;
        if(inverted){
            movingColor *= (-1);
        }
        if(position->GetSquareColor(ind) == movingColor){
            std::list<Move> *m = generators[position->GetSquareValue(ind) + SYMBOLS_OFFSET]->GenerateMoveListVirtual(ind, position);
            moves->splice(moves->end(), *m);
            delete m;
        }
    }
    return moves;
}

void AddPromoMoves(std::list<Move>* moves, Move baseMove, int color){
    baseMove.Promo() = WHITE_ROOK * color;
    moves->push_back(baseMove);
    baseMove.Promo() = WHITE_BISHOP * color;
    moves->push_back(baseMove);
    baseMove.Promo() = WHITE_QUEEN * color;
    moves->push_back(baseMove);
    baseMove.Promo() = WHITE_KNIGHT * color;
    moves->push_back(baseMove);
}
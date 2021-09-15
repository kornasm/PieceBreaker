#include "movecheck.h"
#include "position.h"
#include "move.h"
#include "movegenerators.h"
#include "functions.h"
#include <cmath>

MoveChecker* MoveCheckHandler::checkers[] = {};

void MoveCheckHandler::Init(){
    checkers[0] = new KingMoveChecker();
    checkers[1] = new QueenMoveChecker();
    checkers[2] = new BishopMoveChecker();
    checkers[3] = new KnightMoveChecker();
    checkers[4] = new RookMoveChecker();
    checkers[5] = new BlackPawnMoveChecker();
    checkers[6] = new EmptyMoveChecker();
    checkers[7] = new WhitePawnMoveChecker();
    checkers[8] = new RookMoveChecker();
    checkers[9] = new KnightMoveChecker();
    checkers[10] = new BishopMoveChecker();
    checkers[11] = new QueenMoveChecker();
    checkers[12] = new KingMoveChecker();
    checkers[13] = new EmptyMoveChecker();
}

void MoveCheckHandler::Cleanup(){
    for(auto m : checkers){
        delete m;
    }
}
Move* MoveCheckHandler::CheckMove(const Position& position, int from, int to){
    if(position.GetSquareColor(from) == position.GetSquareColor(to)){
        return NULL;
    }
    if(position.GetSquareValue(to) == OUTSIDE_BOARD){
        return NULL;
    }
    //std::cout << "checkmovehandler  " << position.GetSquareValue(from) << "   " << from << '\n';
    return checkers[position.GetSquareValue(from) + SYMBOLS_OFFSET]->CheckMoveLegality(position, from, to);
}

Move* KingMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    int coldiff = std::abs(column(from) - column(to));
    int rowdiff = std::abs(row(from) - row(to));
    if(coldiff <= 1 && rowdiff <= 1){
        Move* m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareValue(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    if(coldiff >= 3 || rowdiff >= 2){
        return NULL;
    }
    int col = position.GetSquareColor(from);
    if(col == WHITE){
        if(position.WhiteCstl() & SHORT_CASTLE_MOVE){
            if(position.GetSquareColor(from + 1) == EMPTY_SQUARE && position.GetSquareColor(from + 2) == EMPTY_SQUARE){
                std::list<Move>* attackedPlaces = AllMovesGenerator::GenerateMoves(position, true);
                for(auto move : *attackedPlaces){
                    if(move.To() == from || move.To() == from + 1 || move.To() == from + 2){
                        delete attackedPlaces;
                        return NULL;
                    }
                }
                delete attackedPlaces;
                return new Move(from, from + 2, SHORT_CASTLE_MOVE);
            }
        }
        if(position.WhiteCstl() & LONG_CASTLE_MOVE){
            if(position.GetSquareColor(from - 1) == EMPTY_SQUARE && position.GetSquareColor(from - 2) == EMPTY_SQUARE){
                std::list<Move>* attackedPlaces = AllMovesGenerator::GenerateMoves(position, true);
                for(auto move : *attackedPlaces){
                    if(move.To() == from || move.To() == from - 1 || move.To() == from - 2){
                        delete attackedPlaces;
                        return NULL;
                    }
                }
                delete attackedPlaces;
            }
        }
    }
    if(col == BLACK){
        if(position.BlackCstl() & SHORT_CASTLE_MOVE){
            if(position.GetSquareColor(from + 1) == EMPTY_SQUARE && position.GetSquareColor(from + 2) == EMPTY_SQUARE){
                std::list<Move>* attackedPlaces = AllMovesGenerator::GenerateMoves(position, true);
                for(auto move : *attackedPlaces){
                    if(move.To() == from || move.To() == from + 1 || move.To() == from + 2){
                        delete attackedPlaces;
                        return NULL;
                    }
                }
                delete attackedPlaces;
            }
        }
        if(position.BlackCstl() & LONG_CASTLE_MOVE){
            if(position.GetSquareColor(from - 1) == EMPTY_SQUARE && position.GetSquareColor(from - 2) == EMPTY_SQUARE){
                std::list<Move>* attackedPlaces = AllMovesGenerator::GenerateMoves(position, true);
                for(auto move : *attackedPlaces){
                    if(move.To() == from || move.To() == from - 1 || move.To() == from - 2){
                        delete attackedPlaces;
                        return NULL;
                    }
                }
                delete attackedPlaces;
            }
        }
    }
    return NULL;
}

Move* RookMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    if(InBetweenEmpty(position, from, to, true, false)){
        Move *m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareColor(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    return NULL;
}

Move* BishopMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    if(InBetweenEmpty(position, from, to, false, true)){
        Move *m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareColor(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    return NULL;
}

Move* QueenMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    if(InBetweenEmpty(position, from, to, true, true)){
        Move *m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareColor(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    return NULL;
}

Move* KnightMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    int colDiff = std::abs(column(from) - column(to));
    int rowDiff = std::abs(row(from) - row(to));
    if(colDiff != 0 && rowDiff != 0 && colDiff + rowDiff == 3){
        Move *m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareValue(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    return NULL;
}

Move* WhitePawnMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    if(to == from + 10){
        if(position.GetSquareValue(to) == EMPTY_SQUARE){
            return new Move(from, to, PAWN_MOVE);
        }
    }
    if(to == from + 20){
        if(position.GetSquareValue(to) == EMPTY_SQUARE && position.GetSquareValue(from + 10) == EMPTY_SQUARE){
            return new Move(from, to, PAWN_MOVE | PAWN_DOUBLE_MOVE);
        }
    }
    if(to == from + 9 || to == from + 11){
        if(position.GetSquareColor(to) == BLACK){
            return new Move(from, to, PAWN_MOVE | CAPTURE_MOVE);
        }
        if(position.EnPassantPos() == to){
            return new Move(from, to, PAWN_MOVE | EN_PASSANT_MOVE);
        }
    }
    return NULL;
}

Move* BlackPawnMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    if(to == from - 10){
        if(position.GetSquareValue(to) == EMPTY_SQUARE){
            return new Move(from, to, PAWN_MOVE);
        }
    }
    if(to == from - 20){
        if(position.GetSquareValue(to) == EMPTY_SQUARE && position.GetSquareValue(from - 10) == EMPTY_SQUARE){
            return new Move(from, to, PAWN_MOVE | PAWN_DOUBLE_MOVE);
        }
    }
    if(to == from - 9 || to == from - 11){
        if(position.GetSquareColor(to) == WHITE){
            return new Move(from, to, PAWN_MOVE | CAPTURE_MOVE);
        }
    }
    return NULL;
}
#include "movecheck.h"

#include "position.h"
#include "move.h"
#include "movegenerators.h"
#include "board.h"
#include "logger.h"

#include <cmath>

MoveChecker* MoveCheckHandler::checkers[] = {};

extern Logger logger;

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
        return nullptr;
    }
    if(position.GetSquareValue(to) == OUTSIDE_BOARD){
        return nullptr;
    }
    logger << LogDest(LOG_DEBUG) << "checkmovehandler  " << position.GetSquareValue(from) << "   " << from << '\n';
    return checkers[LookUpTableIndex(position.GetSquareValue(from))]->CheckMoveLegality(position, from, to);
}

Move* KingMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    int coldiff = std::abs(Board::column(from) - Board::column(to));
    int rowdiff = std::abs(Board::row(from) - Board::row(to));
    if(coldiff <= 1 && rowdiff <= 1){
        Move* m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareValue(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    if(coldiff >= 3 || rowdiff >= 2){
        return nullptr;
    }
    int col = position.GetSquareColor(from);
    if(col == WHITE){
        if((position.WhiteCstl() & SHORT_CASTLE_MOVE) && to == from + 2){
            if(position.GetSquareColor(from + 1) == EMPTY_SQUARE && position.GetSquareColor(from + 2) == EMPTY_SQUARE){
                std::list<Move> attackedPlaces = AllMovesGenerator::GenerateMoves(position, true);
                for(auto move : attackedPlaces){
                    if(move.To() == from || move.To() == from + 1 || move.To() == from + 2){
                        return nullptr;
                    }
                }
                return new Move(from, from + 2, SHORT_CASTLE_MOVE);
            }
        }
        if((position.WhiteCstl() & LONG_CASTLE_MOVE) && to == from - 2){
            if(position.GetSquareColor(from - 1) == EMPTY_SQUARE && position.GetSquareColor(from - 2) == EMPTY_SQUARE){
                std::list<Move> attackedPlaces = AllMovesGenerator::GenerateMoves(position, true);
                for(auto move : attackedPlaces){
                    if(move.To() == from || move.To() == from - 1 || move.To() == from - 2){
                        return nullptr;
                    }
                }
                return new Move(from, from - 2, LONG_CASTLE_MOVE);
            }
        }
    }
    if(col == BLACK){
        if((position.BlackCstl() & SHORT_CASTLE_MOVE) && to == from + 2){
            if(position.GetSquareColor(from + 1) == EMPTY_SQUARE && position.GetSquareColor(from + 2) == EMPTY_SQUARE){
                std::list<Move> attackedPlaces = AllMovesGenerator::GenerateMoves(position, true);
                for(auto move : attackedPlaces){
                    if(move.To() == from || move.To() == from + 1 || move.To() == from + 2){
                        return nullptr;
                    }
                }
                return new Move(from, from + 2, SHORT_CASTLE_MOVE);
            }
        }
        if((position.BlackCstl() & LONG_CASTLE_MOVE) && to == from - 2){
            if(position.GetSquareColor(from - 1) == EMPTY_SQUARE && position.GetSquareColor(from - 2) == EMPTY_SQUARE){
                std::list<Move> attackedPlaces = AllMovesGenerator::GenerateMoves(position, true);
                for(auto move : attackedPlaces){
                    if(move.To() == from || move.To() == from - 1 || move.To() == from - 2){
                        return nullptr;
                    }
                }
                return new Move(from, from - 2, LONG_CASTLE_MOVE);
            }
        }
    }
    return nullptr;
}

Move* RookMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    if(Board::InBetweenEmpty(position, from, to, true, false)){
        Move *m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareColor(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    return nullptr;
}

Move* BishopMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    if(Board::InBetweenEmpty(position, from, to, false, true)){
        Move *m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareColor(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    return nullptr;
}

Move* QueenMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    if(Board::InBetweenEmpty(position, from, to, true, true)){
        Move *m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareColor(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    return nullptr;
}

Move* KnightMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    int colDiff = std::abs(Board::column(from) - Board::column(to));
    int rowDiff = std::abs(Board::row(from) - Board::row(to));
    if(colDiff != 0 && rowDiff != 0 && colDiff + rowDiff == 3){
        Move *m = new Move(from, to, REGULAR_MOVE);
        if(position.GetSquareValue(to) != EMPTY_SQUARE){
            m->IncreaseType(CAPTURE_MOVE);
        }
        return m;
    }
    return nullptr;
}

Move* WhitePawnMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    Move *m = new Move(from, to, PAWN_MOVE);
    if(Board::row(from) == 7){
        m->IncreaseType(PROMOTION_MOVE);
    }
    if(to == from + 10){
        if(position.GetSquareValue(to) == EMPTY_SQUARE){
            return m;
        }
    }
    if(to == from + 20 && Board::row(from) == 2){
        if(position.GetSquareValue(to) == EMPTY_SQUARE && position.GetSquareValue(from + 10) == EMPTY_SQUARE){
            m->IncreaseType(PAWN_DOUBLE_MOVE);
            return m;
        }
    }
    if(to == from + 9 || to == from + 11){
        if(position.GetSquareColor(to) == BLACK){
            m->IncreaseType(CAPTURE_MOVE);
            return m;
        }
        if(position.EnPassantPos() == to){
            m->IncreaseType(EN_PASSANT_MOVE | CAPTURE_MOVE);
            return m;
        }
    }
    delete m;
    return nullptr;
}

Move* BlackPawnMoveChecker::CheckMoveLegality(const Position& position, int from, int to){
    Move *m = new Move(from, to, PAWN_MOVE);
    if(Board::row(from) == 2){
        m->IncreaseType(PROMOTION_MOVE);
    }
    if(to == from - 10){
        if(position.GetSquareValue(to) == EMPTY_SQUARE){
            return m;
        }
    }
    if(to == from - 20 && Board::row(from) == 7){
        if(position.GetSquareValue(to) == EMPTY_SQUARE && position.GetSquareValue(from - 10) == EMPTY_SQUARE){
            m->IncreaseType(PAWN_DOUBLE_MOVE);
            return m;
        }
    }
    if(to == from - 9 || to == from - 11){
        if(position.GetSquareColor(to) == WHITE){
            m->IncreaseType(CAPTURE_MOVE);
            return m;
        }
        if(position.EnPassantPos() == to){
            m->IncreaseType(EN_PASSANT_MOVE | CAPTURE_MOVE);
            return m;
        }
    }
    delete m;
    return nullptr;
}
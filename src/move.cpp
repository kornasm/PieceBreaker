#include "move.h"

#include "declarations.h"
#include "board.h"
#include "logger.h"

#include <iostream>
#include <cassert>

const char PromotionSymbols[4] = {'q', 'r', 'n', 'b'};
const int  PromotionAnswers[4] = {WHITE_QUEEN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP};

extern Logger logger;

Move::Move(int f, int t, int tp, int pr)
   :from(f),
    to(t),
    type(tp),
    promo(pr)
{}

Move* Move::String2Move(std::string notation){
    if(notation.length() < 4 || notation.length() > 5){
        logger << LogDest(LOG_BAD_INPUT) << "notation must be 4 or 5 (when promoting pawn) kchars long\n";
        return nullptr;
    }
    int from = Board::Not2Ind(notation.substr(0, 2));
    int to = Board::Not2Ind(notation.substr(2, 2));
    int promo(EMPTY_SQUARE);
    if(notation.length() == 5){
        char promosymbol = notation[4];
        for(int i = 0; i < 4; i++){
            if(promosymbol == PromotionSymbols[i]){
                promo = PromotionAnswers[i];
                break;
            }
        }
    }
    if(from < 0 || from > 120){
        logger << LogDest(LOG_BAD_INPUT) << "index out of range\n";
        return nullptr;
    }
    Move *result = new Move(from, to, REGULAR_MOVE, promo);
    return result;
}
 
bool Move::operator ==(Move o) const{
    return (from == o.from && to == o.to);
}

void Move::IncreaseType(int i){
    type |= i;
}

std::ostream& Move::ShowMove(std::ostream& out) const{
    out << "move  " << Board::Ind2Not(From()) << Board::Ind2Not(To());
    out << "     type   " << Type();
    if(Promo() != EMPTY_SQUARE){
        out << "     promo    " << Board::GetPieceSymbol(Promo());
    }
    out << '\n';
    return out;
}

std::string Move::ToString() const{
    std::string ret;
    ret += Board::Ind2Not(this->From());
    ret += Board::Ind2Not(this->To());
    if(this->Promo() != EMPTY_SQUARE){
        char c = Board::GetPieceSymbol(this->Promo());
        if(c <= 'Z'){
            c = static_cast<char>(c + 32);
        }
        ret += c;
    }
    return ret;
}

std::ostream& operator <<(std::ostream& out, const Move& move){
    return out << move.ToString();
}
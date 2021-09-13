#include "declarations.h"
#include "move.h"
#include "functions.h"
int Move::count = 0;
const char PromotionSymbols[4] = {'Q', 'R', 'N', 'B'};
const int  PromotionAnswers[4] = {WHITE_QUEEN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP};

Move::Move(int f, int t, int tp, int pr) :from(f), to(t), type(tp), promo(pr){}

Move* Move::String2Move(std::string notation){
    int from = Not2Ind(notation.substr(0, 2));
    int to = Not2Ind(notation.substr(2, 2));
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
        std::cout << "index out of range\n";
        return NULL;
    }
    Move *result = new Move(from, to, REGULAR_MOVE, promo);
    return result;
}
 
bool Move::operator ==(Move o) const{
    return (from == o.from && to == o.to);
}

void Move::IncreaseType(int i){
    /*int typetemp = type % (2 * i);

    if(typetemp < i){
        type += i;
    }//*/
    if((type & i) == 0){
        type |= i;
    }
}

std::ostream& operator <<(std::ostream& out, Move& move){
    out << "move from   " << Ind2Not(move.from) << "  to  " << Ind2Not(move.to);
    out << "     type   " << move.type;
    if(move.promo != EMPTY_SQUARE){
        out << "     promo    " << GetPieceSymbol(move.promo);
    }
    out << '\n';
    return out;
}
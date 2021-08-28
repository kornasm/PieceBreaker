#include "declarations.h"

char PromotionSymbols[4] = {'R', 'N', 'B', 'Q'};
int  PromotionAnswers[4] = { 2 ,  3 ,  4 ,  5 };

Move::Move(int f, int t, int tp, int pr) :from(f), to(t), type(tp), promo(pr){
}

/*Move::Move(const Move& m) :from(m.from), to(m.to), type(m.type) {

}//*/
Move* Move::String2Move(std::string notation){
    int from = Not2Ind(notation.substr(0, 2));
    int to = Not2Ind(notation.substr(2, 2));
    int promo(EMPTY_SQUARE);
    if(notation.length() == 5){
        char promosymbol = notation[4];
        for(int i = 0; i < 4; i++){
            if(promosymbol == PromotionSymbols[i]){
                promo = PromotionAnswers[i];
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
    int typetemp = type % (2 * i);

    if(typetemp < i){
        type += i;
    }
}

std::ostream& operator <<(std::ostream& out, Move& move){
    out << "move from   " << Ind2Not(move.from) << "  to  " << Ind2Not(move.to) << "     type   " << move.type << '\n';
    return out;
}
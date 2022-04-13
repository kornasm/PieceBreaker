#include "move.h"

#include "declarations.h"
#include "functions.h"

#include <iostream>

const char PromotionSymbols[4] = {'q', 'r', 'n', 'b'};
const int  PromotionAnswers[4] = {WHITE_QUEEN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP};

Move::Move(int f, int t, int tp, int pr) :from(f), to(t), type(tp), promo(pr){}

Move* Move::String2Move(std::string notation){
    if(notation.length() < 4 || notation.length() > 5){
        return nullptr;
    }
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
        return nullptr;
    }
    Move *result = new Move(from, to, REGULAR_MOVE, promo);
    return result;
}
 
bool Move::operator ==(Move o) const{
    return (from == o.from && to == o.to);
}

void Move::IncreaseType(int i){
    if((type & i) == 0){
        type |= i;
    }
}

std::ostream& Move::ShowMove(std::ostream& out) const{
    out << "move  " << Ind2Not(From()) << Ind2Not(To());
    out << "     type   " << Type();
    if(Promo() != EMPTY_SQUARE){
        out << "     promo    " << GetPieceSymbol(Promo());
    }
    out << '\n';
    return out;
}

std::ostream& operator <<(std::ostream& out, const Move& move){
    out << Ind2Not(move.From()) << Ind2Not(move.To());
    if(move.Promo() != EMPTY_SQUARE){
        char c = GetPieceSymbol(move.Promo());
        if(c <= 'Z'){
            c = static_cast<char>(c + 32);
        }
        out << c;
    }
    return out;
}
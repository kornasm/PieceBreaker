#include "declarations.h"

Move::Move(int f, int t, int tp) :from(f), to(t), type(tp) {}

Move::Move(const Move& m) :from(m.from), to(m.to), type(m.type) {}
 
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
#include "declarations.h"

Move::Move(int f, int t, int tp) :from(f), to(t), type(tp) {

};

bool Move::operator ==(Move o) const{
    return (from == o.from && to == o.to);
}

std::ostream& operator <<(std::ostream& out, Move& move){
    out << "move from   " << Ind2Not(move.from) << "  to  " << Ind2Not(move.to) << '\n';
    return out;
}
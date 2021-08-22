#include "declarations.h"

int Move::count = 0;

Move::Move(int f, int t, int tp) :from(f), to(t), type(tp) {
    std::cout << "Move constructor22         " << *this << '\n';
    count++;
};

Move::Move(const Move& m) :from(m.from), to(m.to), type(m.type) {
    std::cout << "Move constructor copy333   " <<  *this << '\n';
    count++;
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
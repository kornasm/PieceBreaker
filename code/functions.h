#include<list>
#include<string>

class MoveGenerator;


extern char PiecesSymbols[13];

char GetPieceSymbol(int piece_number);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

std::string Ind2Not(int index);
int Not2Ind(std::string notation);

bool NotationValid(std::string pos);

int h(std::string hashed);

void init(MoveGenerator* gens[]);
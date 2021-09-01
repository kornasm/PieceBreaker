#include<list>
#include<string>

class MoveGenerator;
class Move;

char GetPieceSymbol(int piece_number);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

std::string Ind2Not(int index);
int Not2Ind(std::string notation);
int row(int position);
int column(int position);

bool NotationValid(std::string pos);

int h(std::string hashed);

void init(MoveGenerator* gens[]);
void cleanup(MoveGenerator* gens[]);

void PrintMoveList(std::list<Move>* moves);
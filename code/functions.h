#include<list>
extern char PiecesSymbols[13];

char GetPieceSymbol(int piece_number);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

std::list<int>* GenerateKingMoves(int position, Node* node);
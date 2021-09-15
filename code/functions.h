#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include<list>
#include<string>

class MoveGenerator;
class Move;
class Position;

char GetPieceSymbol(int piece_number);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

std::string Ind2Not(int index);
int Not2Ind(std::string notation);
int row(int position);
int column(int position);

int ColRow2Ind(int column, int row);

bool NotationValid(std::string pos);

int h(std::string hashed);

void init();
void cleanup();

void PrintMoveList(std::list<Move>* moves);

bool InBetweenEmpty(const Position& pos, int from, int to, bool checkForRook = false, bool checkForBishop = false);

#endif
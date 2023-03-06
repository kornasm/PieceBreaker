#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <list>

class Position;
class Move;

// this class doesn't provide any functionality
// it's just an aggregate of board related functions 
class Board{
    public:
        static char GetPieceSymbol(int piece_number);

        static std::string Ind2Not(int index);
        static int Not2Ind(std::string notation);
        static int row(int position);
        static int column(int position);

        static int ColRow2Ind(int column, int row);

        static bool NotationValid(std::string pos);

        static bool InBetweenEmpty(const Position& pos, int from, int to, bool checkForRook = false, bool checkForBishop = false);
};

// due to lack of better place signum function declared here

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#define SYMBOLS_OFFSET 6
int LookUpTableIndex(int piece);

#endif
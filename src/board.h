#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <list>

class Position;
class Move;

// this class doesn't provide any functionality
// it's just an aggregate of board related functions 
//class Board{

    char GetPieceSymbol(int piece_number);


    std::string Ind2Not(int index);
    int Not2Ind(std::string notation);
    int row(int position);
    int column(int position);

    int ColRow2Ind(int column, int row);

    bool NotationValid(std::string pos);

    bool InBetweenEmpty(const Position& pos, int from, int to, bool checkForRook = false, bool checkForBishop = false);\


    // due to lack of better place signum function declared here

    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
//};








#endif
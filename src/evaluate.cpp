#include "evaluate.h"

#include "declarations.h"
#include "position.h"
#include "functions.h"

#include <iostream>

extern const int mailbox[64];
const double material[NO_PIECES] = {0, -9, -3, -3, -5, -1, 0, 1, 5, 3, 3, 9, 0};
double Evaluator::Evaluate(const Position& position){
    if(position.GetGameResult() != ONGOING){
        if(position.GetGameResult() == DRAW){
            return 0;
        }
        if(position.GetGameResult() == WHITE_WIN){
            return 10000;
        }
        if(position.GetGameResult() == BLACK_WIN){
            return -10000;
        }
    }
    double result = 0;
    for(int i = 0; i < 64; i++){
        int ind = mailbox[i];
        int piece = position.GetSquareValue(ind);
        int pieceind = piece + SYMBOLS_OFFSET;
        result += material[pieceind];
        if(piece == WHITE_PAWN){
            result += ((double)(row(ind) - 2)) / 10;
            // std::cerr << "white pawn  " << ((double)(row(ind) - 2)) / 10 << '\n';
        }
        if(piece == BLACK_PAWN){
            result += ((double)(row(ind) - 7)) / 10;
            // std::cerr << ind << "   " << row(ind) << '\n';
            // std::cerr << "black pawn  " << ((double)(row(ind) - 7)) / 10 << '\n';
        }
        // std::cout << "result   " << result << '\n';
    }
    // std::cout << "result   " << result << '\n';
    return result;
}
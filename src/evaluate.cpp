#include "evaluate.h"

#include "declarations.h"
#include "position.h"
#include "functions.h"
#include "movegenerators.h"
#include "move.h"

#include <iostream>
#include <iomanip>
#include <cmath>

extern const int mailbox[64];
const double material[NO_PIECES] = {0, -9, -3, -3, -5, -1, 0, 1, 5, 3, 3, 9, 0};
float Evaluator::Evaluate(const Position& position){
    //std::cerr << "EVALUATING   \n";
    if(position.GetGameResult() != ONGOING){
        if(position.GetGameResult() == DRAW){
            return 0;
        }
        if(position.GetGameResult() == WHITE_WIN){
            return 12345;
        }
        if(position.GetGameResult() == BLACK_WIN){
            return -12345;
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
    //double plus = (double)(position.ToMove()) / 10;
    int side = position.ToMove();
    //std::cerr << "Material   " << result << '\n';
    std::list<Move> *moves = AllMovesGenerator::GenerateMoves(position);
    std::list<Move> *oppMoves = AllMovesGenerator::GenerateMoves(position, true);
    //std::cerr << "eval    " << result << '\n';
    float pos = (float)((int)(moves->size()) - (int)(oppMoves->size())) / 5 * (float)(position.ToMove());
    result += pos;
    //std::cerr << "eval    " << result << '\n';
    int heatmap[121] = {};
    for(auto move : *moves){
        heatmap[move.To()] += side;
        int movingPiece = position.GetSquareValue(move.From());
        int targetPiece = position.GetSquareValue(move.To());
        if(sgn(targetPiece) == -position.ToMove()){
            if(std::abs(targetPiece) > std::abs(movingPiece)){
                result += (std::abs(targetPiece) - std::abs(movingPiece)) * position.ToMove();
            }
        }
    }
    for(auto move : *oppMoves){
        heatmap[move.To()] += side;
    }
    for(int i = 0; i < 64; i++){
        int color = position.GetSquareColor(mailbox[i]);
        if(sgn(color) != sgn(heatmap[mailbox[i]])){
            result -= color * std::abs(heatmap[mailbox[i]]);
        }
    }//*/
    

    result = round(result * 100) / 100;

    delete moves;
    delete oppMoves;
    return (float)result;
}
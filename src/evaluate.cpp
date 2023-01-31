#include "evaluate.h"

#include "declarations.h"
#include "position.h"
#include "functions.h"
#include "movegenerators.h"
#include "move.h"
#include "logger.h"

#include <iomanip>
#include <cmath>

extern const int mailbox[64];
const double material[NO_PIECES] = {0, -9, -3, -3, -5, -1, 0, 1, 5, 3, 3, 9, 0};
extern Logger logger;

long long Evaluator::hashInfo = 0;
float Evaluator::Evaluate(const Position& position){
    logger << LogDest(LOG_ANALYSIS) << "EVALUATING    " << position.GetPositionHash() << "\n";
    logger << LogDest(LOG_ANALYSIS) << "h    " << hashInfo << '\n';
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
            // logger << LogDest(LOG_DEBUG) << "white pawn  " << ((double)(row(ind) - 2)) / 10 << '\n';
        }
        if(piece == BLACK_PAWN){
            result += ((double)(row(ind) - 7)) / 10;
            // logger << LogDest(LOG_DEBUG) << ind << "   " << row(ind) << '\n';
            // logger << LogDest(LOG_DEBUG) << "black pawn  " << ((double)(row(ind) - 7)) / 10 << '\n';
        }
        logger << LogDest(LOG_ANALYSIS) << "result   " << result << '\n';
    }
    logger << LogDest(LOG_ANALYSIS) << "result   " << result << '\n';
    //double plus = (double)(position.ToMove()) / 10;
    int side = position.ToMove();
    //logger << LogDest(LOG_ANALYSIS) << "Material   " << result << '\n';
    std::list<Move> *moves = AllMovesGenerator::GenerateMoves(position);
    std::list<Move> *oppMoves = AllMovesGenerator::GenerateMoves(position, true);
    //logger << LogDest(LOG_ANALYSIS) << "eval    " << result << '\n';
    float pos = (float)((int)(moves->size()) - (int)(oppMoves->size())) / 5 * (float)(position.ToMove());
    result += pos;
    //logger << LogDest(LOG_ANALYSIS) << "eval    " << result << '\n';
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
        heatmap[move.To()] -= side;
    }
    for(int i = 0; i < 64; i++){
        int color = position.GetSquareColor(mailbox[i]);
        if(sgn(color) != sgn(heatmap[mailbox[i]])){
            result -= color * std::abs(heatmap[mailbox[i]]);
        }
    }//*/
    

    result = round(result * 100) / 100;

    if(position.GetPositionHash() == hashInfo){
        int a = 0;
        a++;
        for(auto m : *moves){
            logger << LogDest(LOG_ANALYSIS) << m << "\n";
        }
        logger << LogDest(LOG_ANALYSIS) << "\n\n";
        for(auto m : *oppMoves){
            logger << LogDest(LOG_ANALYSIS) << m << "\n";
        }
        for(int r = 8; r > 0; r--){
            for(int c = 1; c <= 8; c++){
                if(heatmap[ColRow2Ind(c, r)] >= 0)
                    logger << LogDest(LOG_ANALYSIS) << " ";
                logger << LogDest(LOG_ANALYSIS) << std::fixed << heatmap[ColRow2Ind(c, r)] << " ";
            }
            logger << LogDest(LOG_ANALYSIS) << '\n';
        }
        logger << LogDest(LOG_ANALYSIS) << "\n\n";
    }

    delete moves;
    delete oppMoves;
    return (float)result;
}
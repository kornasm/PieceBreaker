#include "evaluate.h"

#include "declarations.h"
#include "position.h"
#include "functions.h"
#include "movegenerators.h"
#include "move.h"
#include "logger.h"

#include <iomanip>
#include <cmath>
#include <cassert>

extern const int mailbox[64];
const double material[NO_PIECES] = {0, -9, -3, -3, -5, -1, 0, 1, 5, 3, 3, 9, 0};
extern Logger logger;

long long Evaluator::hashInfo = 0;

Evaluator::Evaluator(Position* pos){
    assert(pos != nullptr && "Nullptr was passed to evaluator");
    position = pos;
    
    GeneratePossibleMoves();
    FillHeatMap();
}

Evaluator::~Evaluator(){
    delete moves;
    delete oppMoves;
}

void Evaluator::GeneratePossibleMoves(){
    moves = AllMovesGenerator::GenerateMoves(*position);
    oppMoves = AllMovesGenerator::GenerateMoves(*position, true);
}

void Evaluator::FillHeatMap(){
    for(auto move : *moves){
        heatmap[move.To()] += position->ToMove();

    }
    for(auto move : *oppMoves){
        heatmap[move.To()] -= position->ToMove();
    }
}

double Evaluator::CountMaterial(){
    double result = 0;
    for(int i = 0; i < 64; i++){
        int ind = mailbox[i];
        int piece = position->GetSquareValue(ind);
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

    //logger << LogDest(LOG_ANALYSIS) << "Material   " << result << '\n';
    return result;
}

float Evaluator::Evaluate(){
    logger << LogDest(LOG_ANALYSIS) << "EVALUATING    " << position->GetPositionHash() << "\n";
    logger << LogDest(LOG_ANALYSIS) << "h    " << hashInfo << '\n';
    
    if(position->GetGameResult() != ONGOING){
        return position->GetGameResult();
    }

    double result = 0;

    result += CountMaterial();
    
    float noMoves = (float)((int)(moves->size()) - (int)(oppMoves->size())) / 5 * (float)(position->ToMove());
    result += noMoves;

    // check if weaker piece can capture the stronger one
    for(auto move : *moves){
        int movingPiece = position->GetSquareValue(move.From());
        int targetPiece = position->GetSquareValue(move.To());
        if(sgn(targetPiece) == -position->ToMove()){
            if(std::abs(targetPiece) > std::abs(movingPiece)){
                result += (std::abs(targetPiece) - std::abs(movingPiece)) * position->ToMove();
            }
        }
    }

    // simple board control calculation based on heatmap
    for(int i = 0; i < 64; i++){
        int color = position->GetSquareColor(mailbox[i]);
        if(sgn(color) != sgn(heatmap[mailbox[i]])){
            result -= color * std::abs(heatmap[mailbox[i]]);
        }
    }

    result = round(result * 100) / 100;

    DisplayExtraInfo();

    return (float)result;
}

void Evaluator::DisplayExtraInfo(){
    if(position->GetPositionHash() == hashInfo){
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
}
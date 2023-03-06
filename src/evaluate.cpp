#include "evaluate.h"

#include "declarations.h"
#include "position.h"
#include "board.h"
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
        int pieceind = LookUpTableIndex(piece);
        result += material[pieceind];
        if(piece == WHITE_PAWN){
            result += ((double)(Board::row(ind) - 2)) / 10;
            // logger << LogDest(LOG_DEBUG) << "white pawn  " << ((double)(row(ind) - 2)) / 10 << '\n';
        }
        if(piece == BLACK_PAWN){
            result += ((double)(Board::row(ind) - 7)) / 10;
            // logger << LogDest(LOG_DEBUG) << ind << "   " << row(ind) << '\n';
            // logger << LogDest(LOG_DEBUG) << "black pawn  " << ((double)(row(ind) - 7)) / 10 << '\n';
        }
    }

    //logger << LogDest(LOG_ANALYSIS) << "Material   " << result << '\n';
    return result;
}

double Evaluator::AddEvalForWhite(double val){
    return std::abs(val);
}

double Evaluator::AddEvalForBlack(double val){
    return -std::abs(val);
}

float Evaluator::Evaluate(){
    logger << LogDest(LOG_ANALYSIS) << "EVALUATING    " << position->GetPositionHash() << "\n";
    logger << LogDest(LOG_ANALYSIS) << "h    " << hashInfo << '\n';

    if(position->GetGameResult() != ONGOING){
        return position->GetGameResult();
    }

    double result = 0;

    result += CountMaterial();
    
    logger << LogDest(LOG_ANALYSIS) << "result(material)    " << result << '\n';
    float noMoves = (float)((int)(moves->size()) - (int)(oppMoves->size())) / 10 * (float)(position->ToMove());
    result += noMoves;
    logger << LogDest(LOG_ANALYSIS) << "result(+moves  )    " << result << '\n';

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
    logger << LogDest(LOG_ANALYSIS) << "result(+str/weak)   " << result << '\n';

    // simple board control calculation based on heatmap
    for(int i = 0; i < 64; i++){
        int color = position->GetSquareColor(mailbox[i]);
        if(sgn(color) != sgn(heatmap[mailbox[i]])){
            result -= static_cast<double>(color * std::abs(heatmap[mailbox[i]])) / 5;
        }
    }
    logger << LogDest(LOG_ANALYSIS) << "result(+control)    " << result << '\n';

    // calculate white king safety
    int kingPos = position->GetWhiteKingPos();
    for(auto squareDiff: KingMoveGenerator::KingPossibleSquares){
        double threat = heatmap[kingPos + squareDiff];
        if(sgn(threat) == BLACK){
            result += AddEvalForBlack(std::exp(std::abs(threat) - 1.5 - (position->ToMove() == WHITE)));
        }
        else{
            result += AddEvalForWhite(threat / 5);
        }
    }
    logger << LogDest(LOG_ANALYSIS) << "result(+wKS)        " << result << '\n';

    // calculate black king safety
    kingPos = position->GetBlackKingPos();
    for(auto squareDiff: KingMoveGenerator::KingPossibleSquares){
        double threat = heatmap[kingPos + squareDiff];
        if(sgn(threat) == WHITE){
            result += AddEvalForWhite(std::exp(std::abs(threat) - 1.5 - (position->ToMove() == BLACK)));
        }
        else{
            result += AddEvalForBlack(threat / 5);
        }
    }
    logger << LogDest(LOG_ANALYSIS) << "result(+bKS)        " << result << '\n';

    result = round(result * 100) / 100;

    logger << LogDest(LOG_ANALYSIS) << "result(final)       " << result << '\n';
    DisplayExtraInfo();
    PrintHeat();
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
                if(heatmap[Board::ColRow2Ind(c, r)] >= 0)
                    logger << LogDest(LOG_ANALYSIS) << " ";
                logger << LogDest(LOG_ANALYSIS) << std::fixed << heatmap[Board::ColRow2Ind(c, r)] << " ";
            }
            logger << LogDest(LOG_ANALYSIS) << '\n';
        }
        logger << LogDest(LOG_ANALYSIS) << "\n\n";
    }
}

void Evaluator::PrintHeat(int logLevel){
    for(int row = 8; row > 0; row--){
        logger << LogDest(logLevel) << " ";
        for(int col = 1; col <= 8; col++){
            logger << heatmap[11 + 10 * row + col] << " ";
        }
        logger << '\n';
    }
    logger << '\n';
}
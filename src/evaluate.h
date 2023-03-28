#ifndef EVALUATE_H_
#define EVALUATE_H_

#include "logger.h"

#include <list>
#include <array>

class Position;
class Move;

class Evaluator{
    private:
        const Position *position;
        std::list<Move> moves;
        std::list<Move> oppMoves;
        std::array<int, 121> heatmap = {};

    public:

        static long long hashInfo;
        
        Evaluator(Position* pos);

        float Evaluate();
    
    private:

        void GeneratePossibleMoves();
        void FillHeatMap();
        void DisplayExtraInfo();
        double CountMaterial();
        inline double AddEvalForWhite(double val);
        inline double AddEvalForBlack(double val);

        void PrintHeat(int LogLevel = LOG_ANALYSIS);
};

#endif
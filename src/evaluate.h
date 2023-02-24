#ifndef EVALUATE_H_
#define EVALUATE_H_

#include "logger.h"

#include <list>

class Position;
class Move;

class Evaluator{
    public:

        Evaluator(Position* pos);
        ~Evaluator();
        float Evaluate();
        static long long hashInfo;
    
    private:
        const Position *position;

        std::list<Move> *moves = nullptr;
        std::list<Move> *oppMoves = nullptr;
        int heatmap[121] = {};

        void GeneratePossibleMoves();
        void FillHeatMap();
        void DisplayExtraInfo();

        double CountMaterial();

        inline double AddEvalForWhite(double val);
        inline double AddEvalForBlack(double val);

        void PrintHeat(int LogLevel = LOG_ANALYSIS);
};

#endif
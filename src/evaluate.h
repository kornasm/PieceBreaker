#ifndef EVALUATE_H_
#define EVALUATE_H_

#include "logger.h"

#include <list>

class Position;
class Move;

class Evaluator{
    private:
        const Position *position;
        std::list<Move> *moves = nullptr;
        std::list<Move> *oppMoves = nullptr;
        int heatmap[121] = {};

    public:

        static long long hashInfo;
        
        Evaluator(Position* pos);
        
        ~Evaluator();

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
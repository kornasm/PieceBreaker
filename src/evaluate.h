#ifndef EVALUATE_H_
#define EVALUATE_H_

class Position;
class Move;
#include <list>

class Evaluator{
    public:

        Evaluator(Position* pos);
        ~Evaluator();
        float Evaluate();
        static long long hashInfo;
    
    private:
        Position *position;

        std::list<Move> *moves = nullptr;
        std::list<Move> *oppMoves = nullptr;
        int heatmap[121] = {};

        void GeneratePossibleMoves();
        void FillHeatMap();
        void DisplayExtraInfo();

        double CountMaterial();
};

#endif
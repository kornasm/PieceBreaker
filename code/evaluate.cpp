#include "declarations.h"
#include "position.h"
#include "evaluate.h"   

extern const int mailbox[64];
const int material[NO_PIECES] = {0, -9, -3, -3, -5, -1, 0, 1, 5, 3, 3, 9, 0};
int Evaluator::Evaluate(const Position& position){
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
    int result = 0;
    for(int i = 0; i < 64; i++){
        result += material[position.GetSquareValue(mailbox[i]) + SYMBOLS_OFFSET];
    }
    return result;
}
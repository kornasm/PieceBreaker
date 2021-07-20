#include "declarations.h"

char PiecesSymbols[13] = {'k', 'q', 'b', 'n', 'r', 'p', '-', 'P', 'R', 'N', 'B', 'Q', 'K'};

char GetPieceSymbol(int piece_number){
    return PiecesSymbols[piece_number + SYMBOLS_OFFSET];
    //return '\0';
}

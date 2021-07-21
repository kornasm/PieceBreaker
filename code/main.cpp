#include "declarations.h"

char PiecesSymbols[13] = {'k', 'q', 'b', 'n', 'r', 'p', '-', 'P', 'R', 'N', 'B', 'Q', 'K'};

int main(){
    Node *newgame = new Node();
    newgame->ShowBoard();
    std::list<int>* moves = GenerateKingMoves(42, newgame);
    auto it = moves->begin();
    while(it != moves->end()){
        std::cout << *it << " ";
        ++it;
    }
    delete moves;
    delete newgame;
}
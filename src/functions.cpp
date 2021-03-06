#include "functions.h"

#include "movegenerators.h"
#include "move.h"
#include "movecheck.h"
#include "position.h"
#include "search.h"

#include <cmath>

const char PiecesSymbols[NO_PIECES] = {'k', 'q', 'b', 'n', 'r', 'p', '-', 'P', 'R', 'N', 'B', 'Q', 'K'};

char GetPieceSymbol(int piece_number){
    //std::cerr << "Getting symbol   idx   " << piece_number + SYMBOLS_OFFSET << std::endl;
    return PiecesSymbols[piece_number + SYMBOLS_OFFSET];
}

std::string Ind2Not(int index){
    int column = index % 10 - 1;
    int row = index / 10 - 1;
    char col = (char)(column + 'a' - 1);
    char roww = (char)(row + '0');
    std::string result;
    result += col;
    result += roww;
    return result;
}
int Not2Ind(std::string notation){
    int row = notation[1] - '0' - 1;
    int col = notation[0] - 'a' + 1;
    int result = row * 10 + 20 + col + 1;
    return result;    
}

int row(int position){
    return (position - 10) / 10;
}
int column(int position){
    return position % 10 - 1;
}

int ColRow2Ind(int column, int row){
    return 11 + 10 * row + column;
}


bool NotationValid(std::string pos){
    if(pos.length() != 2){
        return false;
    }
    return (pos[0] >= 'a' && pos[0] <= 'h' && pos[1] >= '1' && pos[1] <= '8');
}

void Init(){
    MoveCheckHandler::Init();
    MoveGeneratorHandler::Init();
    SearchTree::Init();
    return;
}

void Cleanup(){
    MoveGeneratorHandler::Cleanup();
    MoveCheckHandler::Cleanup();
    SearchTree *tree = SearchTree::GetInstance();
    tree->Clear();
    delete tree;
    return;
}

void PrintMoveList(std::list<Move>* moves){
    auto it = moves->begin();
    std::cout << "available places:\n";
    while(it != moves->end()){
        std::cout << "  " << *it;
        it++;
    }
    std::cout << '\n';
}

bool InBetweenEmpty(const Position& pos, int from, int to, bool checkForRook, bool checkForBishop){
    if(from == to){
        return false;
    }
    int colfrom = column(from), colto = column(to);
    int rowfrom = row(from), rowto = row(to);
    int coldiff = column(to) - column(from);
    int rowdiff = row(to) - row(from);
    if(coldiff != 0 && rowdiff != 0 && std::abs(coldiff) != std::abs(rowdiff)){
        return false;
    }
    if(checkForRook == false){
        if(coldiff == 0 || rowdiff == 0){
            return false;
        }
    }
    if(checkForBishop == false){
        if(std::abs(coldiff) == std::abs(rowdiff)){
            return false;
        }
    }
    int coltemp = colfrom, rowtemp = rowfrom;
    int dircol = sgn(coldiff), dirrow = sgn(rowdiff);
    while(true){
        coltemp += dircol;
        rowtemp += dirrow;
        if(coltemp == colto && rowtemp == rowto){
            return true;
        }
        if(pos.GetSquareColor(coltemp, rowtemp) != EMPTY_SQUARE){
            return false;
        }
    }
}
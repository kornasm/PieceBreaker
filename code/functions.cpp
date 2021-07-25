#include "declarations.h"

char GetPieceSymbol(int piece_number){
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

bool NotationValid(std::string pos){
    std::cout << '#' << pos << '#' << '\n';
    if(pos.length() != 2){
        std::cout << "notation invalid,  length = " << pos.length() << '\n';
        return false;
    }
    //std:: cout << (int)('a') << " " << (int)(pos[0]) << " " << (int)('h') << " ";
    //std:: cout << (int)('1') << " " << (int)(pos[1]) << " " << (int)('8') << '\n';
    return (pos[0] >= 'a' && pos[0] <= 'h' && pos[1] >= '1' && pos[1] <= '8');
}

int row(int position){
    return (position - 10) / 10;
}
int column(int position){
    return position % 10 - 1;
}

int h(std::string hashed){
    long long sum = 0;
    for(unsigned int i = 0; i < hashed.length(); i++){
        sum = sum * 311;
        sum += (int)(hashed[i]);
        sum %= 1000000000 + 7;
    }
    return sum;
}

void init(MoveGenerator* gens[]){
    gens[0] = new KingMoveGenerator();
    gens[1] = new QueenMoveGenerator();
    gens[2] = new BishopMoveGenerator();
    gens[3] = new KnightMoveGenerator();
    gens[4] = new RookMoveGenerator();
    gens[5] = new BlackPawnMoveGenerator();
    gens[6] = new EmptyMoveGenerator();
    gens[7] = new WhitePawnMoveGenerator();
    gens[8] = new RookMoveGenerator();
    gens[9] = new KnightMoveGenerator();
    gens[10] = new BishopMoveGenerator();
    gens[11] = new QueenMoveGenerator();
    gens[12] = new KingMoveGenerator();
    return;
}

void cleanup(MoveGenerator* gens[]){
    for(int i = 0; i < NO_PIECES; i++){
        delete gens[i];
    }
    return;
}
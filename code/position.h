#ifndef POSITION_H_
#define POSITION_H_

#include<string>
#include<list>
#include"declarations.h"

enum GameResult{ONGOING, WHITE_WIN, DRAW, BLACK_WIN};
class Move;
class MoveGenerator;
extern MoveGenerator* generators[NO_PIECES + 1];

class Position{
    public:
        int squares[121] = {7,
                             7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                             7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                             7, 2, 3, 4, 5, 6, 4, 3, 2, 7,
                             7, 1, 1, 1, 1, 1, 1, 1, 1, 7,
                             7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                             7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                             7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                             7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                             7, -1, -1, -1, -1, -1, -1, -1, -1, 7,
                             7, -2, -3, -4, -5, -6, -4, -3, -2, 7,
                             7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                             7, 7, 7, 7, 7, 7, 7, 7, 7, 7
        };
        int toMove;
        int whcstl;
        int blcstl;
        int enPassant;
        bool underCheck;
        Position *prev;
        GameResult result;
        long long positionHash;
        bool searchBackRepetitions;
        int halfMoveClock;
        int fullMoveCounter;
        int whiteKingPos;
        int blackKingPos;

        Position();
        Position(Position *prev, Move *m, int promo = 0, bool execute = true);
        Position(std::string fen);
        ~Position();

        Position(const Position& position) = delete;
        Position operator=(const Position& position) = delete;
        
        void ShowBoard() const;
        Position* MakeMove(Move* checkedmove, bool execute = true);
        Move* CheckIfMoveFullLegal(Move* checkedmove, bool pseudoLegalWarranty = false);
        Move* CheckIfMovePseudoLegal(int from, int to);
        int MakeSoftMove(Move *toExecute);
        void MakeSoftBack(Move *toExecute, int takenPiece);
        
        
        void CheckCheck();
        std::list<Move>* GenerateAllLegalMoves();
        bool CheckIfMoveLegal(Move move);
        void CheckEndings();

        void CalculatePositionHash();


        char GetPiece(int column, int row) const;
        int GetSquareValue(int column, int row) const;
        int GetSquareColor(int column, int row) const;
        char GetPiece(int index) const;
        int GetSquareValue(int index) const;
        int GetSquareColor(int index) const;

        bool IsPlaceAttacked(int attackedplace, int atackingcolor);
};

#endif

/*

0
1    2  3  4  5  6  7  8  9    10   
11   12 13 14 15 16 17 18 19   20

21   22 23 24 25 26 27 28 29   30
31   32 33 34 35 36 37 38 39   40
41   42 43 44 45 46 47 48 49   50
51   52 53 54 55 56 57 58 59   60
61   62 63 64 65 66 67 68 69   70
71   72 73 74 75 76 77 78 79   80
81   82 83 84 85 86 87 88 89   90
91   92 93 94 95 96 97 98 99   100

101 102 103 104 105 106 107 108 109 110
111 112 113 114 115 116 117 118 119 120
*/
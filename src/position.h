#ifndef POSITION_H_
#define POSITION_H_

#include "declarations.h"

#include <string>
#include <list>
#include <iostream>
#include <array>

const std::array<int, 64> mailbox = {22, 23, 24, 25, 26, 27, 28, 29,
                                     32, 33, 34, 35, 36, 37, 38, 39,
                                     42, 43, 44, 45, 46, 47, 48, 49,
                                     52, 53, 54, 55, 56, 57, 58, 59,
                                     62, 63, 64, 65, 66, 67, 68, 69,
                                     72, 73, 74, 75, 76, 77, 78, 79,
                                     82, 83, 84, 85, 86, 87, 88, 89,
                                     92, 93, 94, 95, 96, 97, 98, 99};
                                     
enum GameResult{ONGOING = 1, WHITE_WIN = 10000, DRAW = 0, BLACK_WIN = -10000};
class Move;
class MoveGenerator;

class Position{
    private:
        std::array<int, 121> squares = {7,//A,B, C, D, E, F, G, H
                                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                            /*  1 */    7, 2, 3, 4, 5, 6, 4, 3, 2, 7,
                            /*  2 */    7, 1, 1, 1, 1, 1, 1, 1, 1, 7,
                            /*  3 */    7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                            /*  4 */    7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                            /*  5 */    7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                            /*  6 */    7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                            /*  7 */    7, -1, -1, -1, -1, -1, -1, -1, -1, 7,
                            /*  8 */    7, -2, -3, -4, -5, -6, -4, -3, -2, 7,
                                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
        int toMove;
        int whcstl;
        int blcstl;
        int enPassantPosition;
        bool underCheck;
        Position *prev;
        GameResult result;
        long long positionHash;
        bool searchBackRepetitions;
        int halfMoveClock;
        int fullMoveCounter;
        int whiteKingPos;
        int blackKingPos;

    public:
        Position();
        Position(Position& prev, Move *m, int promo = 0);
        Position(std::stringstream& issfen);
        Position(const Position& position) = delete;
        Position operator=(const Position& position) = delete;

        ~Position();

        
        // move making/checking
        Position* MakeMove(Move* checkedmove);
        Move* CheckIfMoveFullLegal(Move* checkedmove, bool pseudoLegalWarranty = false);
        Move* CheckIfMovePseudoLegal(int from, int to) const;
        int MakeSoftMove(Move *toExecute);
        void MakeSoftBack(Move *toExecute, int takenPiece);
        bool IsPlaceAttacked(int attackedplace, int atackingcolor) const;
        
        void ShowBoard(std::ostream& out) const;
        std::ostream& ShowTinyBoard(std::ostream& out) const;
        bool CheckCheck();
        std::list<Move> GenerateAllLegalMoves(bool searchAtLeastOne = false);
        GameResult CheckEndings();
        long long CalculatePositionHash();

        //getters
        std::string GetFen() const;
        char GetPiece(int column, int row) const;
        int GetSquareValue(int column, int row) const;
        int GetSquareColor(int column, int row) const;
        char GetPiece(int index) const;
        int GetSquareValue(int index) const;
        int GetSquareColor(int index) const;
        int WhiteCstl() const { return whcstl; }
        int BlackCstl() const { return blcstl; }
        int GetWhiteKingPos() const { return whiteKingPos; }
        int GetBlackKingPos() const { return blackKingPos; }
        bool IsUnderCheck() const { return underCheck; }
        int EnPassantPos() const { return enPassantPosition; }
        int ToMove() const { return toMove; }
        GameResult GetGameResult() const { return result; }
        long long GetPositionHash() const { return positionHash; }
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
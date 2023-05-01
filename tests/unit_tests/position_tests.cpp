#include <gtest/gtest.h>

#include <iostream>
#include <array>
#include <vector>
#include <memory>

#include "board.h"
#include "position.h"
#include "move.h"
#include "node.h"
#include "movegenerators.h"
#include "movecheck.h"

class PositionTests: public ::testing::Test {
    protected:
        PositionTests() {
            MoveCheckHandler::Init();
            MoveGeneratorHandler::Init();
        }
        ~PositionTests() override {
            MoveCheckHandler::Cleanup();
            MoveGeneratorHandler::Cleanup();
        }
};

TEST_F(PositionTests, FenReadingPrinting){
    std::array<std::string, 4> fens = { "rn3bn1/pbppk1pp/1p2p2r/2q1Np2/8/2P1PK2/P1PP1PPP/RNBQ1B1R w - - 0 1",
                                      "4k3/Q7/4K3/8/8/8/8/8 w - - 0 1",
                                      "4k3/Q7/4K3/8/8/8/8/8 b - - 10 115",
                                      "rnbqkb1r/pp3ppp/3p1n2/2pPp3/4P3/5N2/PPP2PPP/RNBQKB1R w KQkq c6 38 75" };
    for(auto inFen : fens){
        std::stringstream sstr(inFen);
        std::unique_ptr<Position> pos(new Position(sstr));
        std::string outFen = pos->GetFen();
        EXPECT_EQ(inFen, outFen) << "Test:  " << inFen << "  failed\n";
        //std::cerr << "#" << inFen << "#\n#" << outFen << "#\n";
    }
}

TEST_F(PositionTests, IsPlaceAttacked){
    std::array<std::string, 2> fens = {"8/6k1/6b1/2Bq3Q/1b2Pp2/2P3N1/1R2K2P/8 w - - 0 1",
                                       "3k1nn1/4p1nb/2q2pr1/P1b1Q3/1P6/2P3N1/8/4K3 b - - 0 1"
                                    };
    std::array<std::vector<std::string>, 2> attackedPlaces =
    {
        std::vector<std::string> {
            "b1", "d1", "e1", "f1", "g1", "h1", "a2", "c2", "d2", "f2", "b3", "d3", "e3", "f3", "h3", "b4", "c4", "d4", "g4", "h4", "d5", "e5", "f5", "g5", "b6", "d6", "g6", "h6", "a7", "e7", "h7", "f8", "h8" },
        std::vector<std::string> {
            "g1", "h1", "f2", "g2", "e3", "f3", "g3", "a4", "b4", "d4", "e4", "g4", "b5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "d6", "e6", "h6", "a7", "b7", "c7", "d7", "a8", "c8", "e8" }
    };
    
    std::array<std::vector<std::string>, 2> safePlaces =
    {
        std::vector<std::string> {
            "a1", "c1", "b2", "e2", "g2", "h2", "a3", "c3", "g3", "a4", "e4", "f4", "a5", "b5", "c5", "a6", "c6", "e6", "f6", "b7", "c7", "d7", "f7", "g7", "a8", "b8", "c8", "d8", "e8", "g8" },
        std::vector<std::string> {
            "a1", "b1", "c1", "d1", "e1", "f1", "a2", "b2", "c2", "d2", "e2", "h2", "a3", "b3", "c3", "d3", "h3", "c4", "f4", "h4", "a5", "c5", "c6", "f6", "g6", "e7", "f7", "g7", "h7", "b8", "d8", "f8", "g8", "h8" }
    };
    for(unsigned int idx = 0; idx < fens.size(); idx++){
        std::stringstream sstr(fens[idx]);
        std::unique_ptr<Position> pos(new Position(sstr));
        for(auto square_str: attackedPlaces[idx]){
            int square = Board::Not2Ind(square_str);
            bool isAttacked = pos->IsPlaceAttacked(square, pos->ToMove());
            EXPECT_EQ(isAttacked, true) << square_str;
        }
        for(auto square_str: safePlaces[idx]){
            int square = Board::Not2Ind(square_str);
            bool isAttacked = pos->IsPlaceAttacked(square, pos->ToMove());
            EXPECT_EQ(isAttacked, false) << square_str;
        }
    }
}

// remember that color to move is being investigated to be under check
TEST_F(PositionTests, DetectCheck){
    std::array<std::string, 6> fens {
        "3k4/8/8/6Q1/8/8/4K3/8 b - - 0 1",
        "2k1r3/8/8/2b5/8/4K3/8/8 w - - 0 1",
        "3k4/r3nN2/3q1p1b/6p1/R1Q1B1Bp/8/4B3/1KR5 b - - 0 1",
        "5k2/r3nNb1/3q1ppb/5B2/R1Q1B1Bp/3R4/8/2K5 w - - 0 1",
        "1k6/3BNb2/8/6n1/1q5K/1rr1R3/5nnn/3q1b1b w - - 0 1",
        "8/1q3kp1/2pp3p/1p4b1/8/8/B5PP/1K1QR3 b - - 0 1"
    };

    for(unsigned int i = 0; i < fens.size(); i++){
        std::cerr << "full fen     " << fens[i] << "\n\n";
        std::stringstream sstr(fens[i]);
        std::unique_ptr<Position> pos(new Position(sstr));
        bool check = pos->IsUnderCheck();
        EXPECT_EQ(check, true) << "check is not recognized in  " << fens[i];
    }
}

TEST_F(PositionTests, DetectNoCheck){
    std::array<std::string, 7> fens {
        "rnbq1bnr/pppppppp/6k1/8/8/6K1/PPPPPPPP/RNBQ1BNR w - - 0 1",
        "2kb4/4n1p1/8/1rb3K1/8/4r3/N2q2n1/6q1 w - - 0 1",
        "4q3/3PkP2/2P1P1P1/3P1P2/8/8/3QKN2/6B1 b - - 0 1",
        "3k4/r3nn2/3qpppb/5N2/4B3/1K4B1/4Q1R1/2B5 b - - 0 1",
        "3k4/r3nn2/3qpppb/5N2/4B3/1K4B1/4Q1R1/2B5 w - - 0 1",
        "r5k1/3q1n1p/2b3r1/7p/2nP4/1NP3RP/1QQ2N2/4KB2 b - - 0 1",
        "1k6/5b2/8/4q1n1/7K/1rr5/5n1n/3q1bnb w - - 0 1"

    };
    for(unsigned int i = 0; i < fens.size(); i++){
        
        std::stringstream sstr(fens[i]);
        std::unique_ptr<Position> pos(new Position(sstr));
        bool check = pos->IsUnderCheck();
        EXPECT_EQ(check, false) << "check is incorectly recognized in  " << std::endl << fens[i];
    }
}

TEST_F(PositionTests, GettersTest){
    std::string fen = "r4rk1/pbqn1p1p/1ppbpn2/3p2pP/3P2P1/1PN1PP1N/PBPQ4/2KR1B1R w Kq g6 0 2";
    std::stringstream sstr(fen);
    std::unique_ptr<Position> pos(new Position(sstr));

    EXPECT_EQ(pos->GetPiece(3, 1), 'K');
    EXPECT_EQ(pos->GetPiece(8, 5), 'P');
    EXPECT_EQ(pos->GetPiece(4, 6), 'b');
    EXPECT_EQ(pos->GetSquareValue(2, 6), BLACK_PAWN);
    EXPECT_EQ(pos->GetSquareValue(6, 6), BLACK_KNIGHT);
    EXPECT_EQ(pos->GetSquareValue(6, 1), WHITE_BISHOP);
    EXPECT_EQ(pos->GetSquareValue(6, 4), EMPTY_SQUARE);
    EXPECT_EQ(pos->GetSquareColor(1, 7), BLACK);
    EXPECT_EQ(pos->GetSquareColor(4, 1), WHITE);
    EXPECT_EQ(pos->GetSquareColor(7, 8), BLACK);
    EXPECT_EQ(pos->GetSquareColor(8, 8), EMPTY_SQUARE);
    EXPECT_EQ(pos->GetPiece(Board::Not2Ind("b6")), 'p');
    EXPECT_EQ(pos->GetPiece(Board::Not2Ind("e3")), 'P');
    EXPECT_EQ(pos->GetPiece(Board::Not2Ind("a4")), '-');
    EXPECT_EQ(pos->GetSquareValue(Board::Not2Ind("f3")), WHITE_PAWN);
    EXPECT_EQ(pos->GetSquareValue(Board::Not2Ind("c7")), BLACK_QUEEN);
    EXPECT_EQ(pos->GetSquareValue(Board::Not2Ind("c6")), BLACK_PAWN);
    EXPECT_EQ(pos->GetSquareColor(Board::Not2Ind("c4")), EMPTY_SQUARE);
    EXPECT_EQ(pos->GetSquareColor(Board::Not2Ind("d5")), BLACK);
    EXPECT_EQ(pos->GetSquareColor(Board::Not2Ind("c1")), WHITE);
    EXPECT_EQ(pos->GetWhiteKingPos(), Board::Not2Ind("c1"));
    EXPECT_EQ(pos->GetBlackKingPos(), Board::Not2Ind("g8"));
    EXPECT_EQ(pos->EnPassantPos(), Board::Not2Ind("g6"));
    EXPECT_EQ(pos->ToMove(), WHITE);
    EXPECT_EQ(pos->GetGameResult(), GameResult::ONGOING);
}
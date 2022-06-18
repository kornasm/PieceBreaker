#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <list>
#include <string>

#include "../src/functions.h"
#include "../src/node.h"
#include "../src/move.h"
#include "../src/position.h"

//bool CkeckMoves(Position &position, std::list<Move> moves, bool isMoveValid, int moveType = 0){
//    EXPECT_EQ(1, 1)
//}


TEST(MoveTests, MoveToEmptySquare){
    // these tests contain only simple moves
    // no taking other pieces, no castles, no pawn moves
    Init();
    std::string whiteFen = "4n3/3p2k1/5pb1/6r1/2P5/1K2BN1P/1Q4Pq/3R4 w - - 0 1";
    std::string blackFen = "4n3/3p2k1/5pb1/6r1/2P5/1K2BN1P/1Q4Pq/3R4 b - - 0 1";
    std::list<Move> whiteValidMoves;
    std::list<Move> blackValidMoves;
    // white king
    whiteValidMoves.push_back(Move(Not2Ind("b3"), Not2Ind("b4"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("b3"), Not2Ind("a4"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("b3"), Not2Ind("c3"), 0));
    // white queen
    whiteValidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("d4"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("f2"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("e2"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("c1"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("b1"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("a1"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("a2"), 0));
    // white rook
    whiteValidMoves.push_back(Move(Not2Ind("d1"), Not2Ind("d6"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("d1"), Not2Ind("d3"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("d1"), Not2Ind("g1"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("d1"), Not2Ind("a1"), 0));
    // white bishop
    whiteValidMoves.push_back(Move(Not2Ind("e3"), Not2Ind("c1"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("e3"), Not2Ind("b6"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("e3"), Not2Ind("f4"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("e3"), Not2Ind("g1"), 0));
    // white knight
    whiteValidMoves.push_back(Move(Not2Ind("f3"), Not2Ind("e5"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("f3"), Not2Ind("h4"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("f3"), Not2Ind("g1"), 0));
    whiteValidMoves.push_back(Move(Not2Ind("f3"), Not2Ind("e1"), 0));
    // white pawns
    //whiteValidMoves.push_back(Move(Not2Ind("c4"), Not2Ind("c5"), 0));
    //whiteValidMoves.push_back(Move(Not2Ind("g2"), Not2Ind("g3"), 0));
    //whiteValidMoves.push_back(Move(Not2Ind("h3"), Not2Ind("h4"), 0));

    // black king
    blackValidMoves.push_back(Move(Not2Ind("g7"), Not2Ind("f8"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g7"), Not2Ind("g8"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g7"), Not2Ind("h7"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g7"), Not2Ind("h6"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g7"), Not2Ind("f7"), 0));
    // black queen
    blackValidMoves.push_back(Move(Not2Ind("h2"), Not2Ind("h1"), 0));
    blackValidMoves.push_back(Move(Not2Ind("h2"), Not2Ind("g1"), 0));
    blackValidMoves.push_back(Move(Not2Ind("h2"), Not2Ind("g3"), 0));
    blackValidMoves.push_back(Move(Not2Ind("h2"), Not2Ind("c7"), 0));
    // black rook
    blackValidMoves.push_back(Move(Not2Ind("g5"), Not2Ind("h5"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g5"), Not2Ind("g3"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g5"), Not2Ind("e5"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g5"), Not2Ind("a5"), 0));
    // black bishop
    blackValidMoves.push_back(Move(Not2Ind("g6"), Not2Ind("h7"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g6"), Not2Ind("h5"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g6"), Not2Ind("b1"), 0));
    blackValidMoves.push_back(Move(Not2Ind("g6"), Not2Ind("f7"), 0));
    // black knight
    blackValidMoves.push_back(Move(Not2Ind("e8"), Not2Ind("d6"), 0));
    blackValidMoves.push_back(Move(Not2Ind("e8"), Not2Ind("c7"), 0));

    std::stringstream sstr(whiteFen);
    std::unique_ptr<Position> position(new Position(sstr));

    for(auto move : whiteValidMoves){

        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        ASSERT_NE(result.get(), nullptr) << "white Valid Move is null   " << result.get() << "   " << move << std::endl;
        EXPECT_EQ(result->Type(), REGULAR_MOVE) << "white Valid Move is wrong type   " << result.get() << "   " << move << std::endl;
    }
    sstr = std::stringstream(blackFen);
    position = std::unique_ptr<Position>(new Position(sstr));
    for(auto move : blackValidMoves){
        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        ASSERT_NE(result.get(), nullptr) << "valid move failed" << std::endl;
        EXPECT_EQ(result->Type(), REGULAR_MOVE);
    }
    Cleanup();
}

TEST(MoveTests, CaptureMoves){
    // still no pawn moves
    Init();
    std::string whiteFen = "kB6/pppq1p1p/4QBpP/1N1P4/2Rn1r2/3NPb2/P1P1q1P1/Kn6 w - - 0 1";
    std::string blackFen = "kB6/pppq1p1p/4QBpP/1N1P4/2Rn1r2/3NPb2/P1P1q1P1/Kn6 b - - 0 1";

    std::list<Move> whiteMoves;
    std::list<Move> blackMoves;

    whiteMoves.push_back(Move(Not2Ind("b5"), Not2Ind("c7"), 0));
    whiteMoves.push_back(Move(Not2Ind("b5"), Not2Ind("d4"), 0));
    whiteMoves.push_back(Move(Not2Ind("c4"), Not2Ind("c7"), 0));
    whiteMoves.push_back(Move(Not2Ind("c4"), Not2Ind("d4"), 0));
    whiteMoves.push_back(Move(Not2Ind("d3"), Not2Ind("f4"), 0));
    whiteMoves.push_back(Move(Not2Ind("e6"), Not2Ind("d7"), 0));
    whiteMoves.push_back(Move(Not2Ind("e6"), Not2Ind("f7"), 0));
    whiteMoves.push_back(Move(Not2Ind("f6"), Not2Ind("d4"), 0));
    whiteMoves.push_back(Move(Not2Ind("a1"), Not2Ind("b1"), 0));
    whiteMoves.push_back(Move(Not2Ind("b8"), Not2Ind("c7"), 0));

    blackMoves.push_back(Move(Not2Ind("a8"), Not2Ind("b8"), 0));
    blackMoves.push_back(Move(Not2Ind("d7"), Not2Ind("b5"), 0));
    blackMoves.push_back(Move(Not2Ind("d7"), Not2Ind("d5"), 0));
    blackMoves.push_back(Move(Not2Ind("d7"), Not2Ind("e6"), 0));
    blackMoves.push_back(Move(Not2Ind("d4"), Not2Ind("b5"), 0));
    blackMoves.push_back(Move(Not2Ind("d4"), Not2Ind("e6"), 0));
    blackMoves.push_back(Move(Not2Ind("d4"), Not2Ind("c2"), 0));
    blackMoves.push_back(Move(Not2Ind("f4"), Not2Ind("f6"), 0));
    blackMoves.push_back(Move(Not2Ind("f3"), Not2Ind("g2"), 0));
    blackMoves.push_back(Move(Not2Ind("f3"), Not2Ind("d5"), 0));
    blackMoves.push_back(Move(Not2Ind("e2"), Not2Ind("d3"), 0));

    std::stringstream sstr(whiteFen);
    std::unique_ptr<Position> position(new Position(sstr));
    for(auto move : whiteMoves){
        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        ASSERT_NE(result.get(), nullptr) << "white Valid Move is null   " << result.get() << "   " << move << std::endl;
        EXPECT_EQ(result->Type(), CAPTURE_MOVE) << "white Valid Move is wrong type   " << result.get() << "   " << move << std::endl;
    }
    
    sstr = std::stringstream(blackFen);
    position = std::unique_ptr<Position>(new Position(sstr));
    for(auto move : blackMoves){
        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        ASSERT_NE(result.get(), nullptr) << "black valid move failed" << std::endl;
        EXPECT_EQ(result->Type(), CAPTURE_MOVE);
    }
    Cleanup();
}

TEST(MoveTests, InvalidMoves){
    // these moves should be considered as invalid
    Init();
    std::string whiteFen = "4n3/3p2k1/5pb1/6r1/2P5/1K2BN1P/1Q4Pq/3R4 w - - 0 1";
    std::string blackFen = "4n3/3p2k1/5pb1/6r1/2P5/1K2BN1P/1Q4Pq/3R4 b - - 0 1";

    std::list<Move> whiteInvalidMoves;
    std::list<Move> blackInvalidMoves;

    // just an invalid move
    whiteInvalidMoves.push_back(Move(Not2Ind("b3"), Not2Ind("b6"), 0));
    whiteInvalidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("g5"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("d7"), Not2Ind("d4"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("d7"), Not2Ind("e6"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("e8"), Not2Ind("a1"), 0));
    // capturing own piece
    whiteInvalidMoves.push_back(Move(Not2Ind("b3"), Not2Ind("c4"), 0));
    whiteInvalidMoves.push_back(Move(Not2Ind("b3"), Not2Ind("b2"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("g7"), Not2Ind("g6"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("g5"), Not2Ind("g6"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("e8"), Not2Ind("f6"), 0));
    // ommiting another piece
    whiteInvalidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("g7"), 0));
    whiteInvalidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("h8"), 0));
    whiteInvalidMoves.push_back(Move(Not2Ind("e3"), Not2Ind("h6"), 0));
    // outside board (those should not cause SegFault)
    whiteInvalidMoves.push_back(Move(Not2Ind("b2"), Not2Ind("c0"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("g5"), Not2Ind("i5"), 0));
    // moving piece from empty square
    whiteInvalidMoves.push_back(Move(Not2Ind("a2"), Not2Ind("a3"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("e5"), Not2Ind("e4"), 0));

    std::stringstream sstr(whiteFen);
    std::unique_ptr<Position> position(new Position(sstr));

    for(auto move : whiteInvalidMoves){
        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        EXPECT_EQ(result.get(), nullptr);
    }
    sstr = std::stringstream(blackFen);
    position = std::unique_ptr<Position>(new Position(sstr));

    for(auto move : blackInvalidMoves){
        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        EXPECT_EQ(result.get(), nullptr);
    }    
    Cleanup();

}

TEST(MoveTests, PawnMoves){
    Init();
    std::string whiteFen = "k7/pp5p/6p1/1NpP4/5r2/2P1P2b/P1P3P1/K7 w - c6 0 2";
    std::string blackFen = "k7/ppp1p2p/8/1N1P4/7b/2P1P3/P1P3P1/K7 b - - 0 1";

    std::list<Move> whiteRegularMoves;
    std::list<Move> whiteDoubleMoves;
    std::list<Move> whiteEnPassantMoves;

    whiteRegularMoves.push_back(Move(Not2Ind("a2"), Not2Ind("a3"), 0));
    whiteRegularMoves.push_back(Move(Not2Ind("c3"), Not2Ind("c4"), 0));
    whiteRegularMoves.push_back(Move(Not2Ind("d5"), Not2Ind("d6"), 0));
    whiteRegularMoves.push_back(Move(Not2Ind("e3"), Not2Ind("e4"), 0));
    whiteRegularMoves.push_back(Move(Not2Ind("g2"), Not2Ind("g3"), 0));

    whiteDoubleMoves.push_back(Move(Not2Ind("a2"), Not2Ind("a4"), 0));
    whiteDoubleMoves.push_back(Move(Not2Ind("g2"), Not2Ind("g4"), 0));

    whiteEnPassantMoves.push_back(Move(Not2Ind("d5"), Not2Ind("c6"), 0));

    std::stringstream sstr(whiteFen);
    std::unique_ptr<Position> position(new Position(sstr));

    for(auto move : whiteRegularMoves){
        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        ASSERT_NE(result.get(), nullptr) << "valid move failed" << std::endl;
        EXPECT_EQ(result->Type(), PAWN_MOVE) << move;
    }
    for(auto move : whiteDoubleMoves){
        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        ASSERT_NE(result.get(), nullptr) << "valid move failed" << std::endl;
        EXPECT_EQ(result->Type(), PAWN_MOVE | PAWN_DOUBLE_MOVE) << move;
    }
    for(auto move : whiteEnPassantMoves){
        std::unique_ptr<Move> result(position->CheckIfMoveFullLegal(&move));
        ASSERT_NE(result.get(), nullptr) << "valid move failed" << std::endl;
        EXPECT_EQ(result->Type(), PAWN_MOVE | EN_PASSANT_MOVE | CAPTURE_MOVE) << move;
    }
    Cleanup();
}//*/
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

class MoveValidationTests: public ::testing::Test {
    protected:
        MoveValidationTests() {
           PieceBreaker::Init(0, nullptr);
        }
        ~MoveValidationTests() override {
           PieceBreaker::Cleanup();
        }
};

bool testMoves(std::string fen, std::list<Move> moves, bool resultShouldBeNull, int expectedMoveType, bool checkType = true){
    std::stringstream sstr(fen);
    std::unique_ptr<Position> position(new Position(sstr));
    for(auto move : moves){
        std::unique_ptr<Move> result = std::unique_ptr<Move>(position->CheckIfMoveFullLegal(&move));
        if(resultShouldBeNull == false){
            // ASSERT result is not null
            EXPECT_NE(result.get(), nullptr) << move << "   is null\n";
            if(result.get() == nullptr){
                return false;
            }
            if(checkType){
                EXPECT_EQ(result->Type(), expectedMoveType) << move << "  has wrong type\n";
            }
        }
        else{
            EXPECT_EQ(result.get(), nullptr) << move << "  isn't null\n";
        }
    }
    return true;
}


TEST_F(MoveValidationTests, MoveToEmptySquare){
    // these tests contain only simple moves
    // no taking other pieces, no castles, no pawn moves

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

    testMoves(whiteFen, whiteValidMoves, false, REGULAR_MOVE);
    testMoves(blackFen, blackValidMoves, false, REGULAR_MOVE);
}

TEST_F(MoveValidationTests, CaptureMoves){
    // still no pawn moves

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

    testMoves(whiteFen, whiteMoves, false, CAPTURE_MOVE);
    testMoves(blackFen, blackMoves, false, CAPTURE_MOVE);
}

TEST_F(MoveValidationTests, InvalidMoves){
    // these moves should be considered as invalid

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

    testMoves(whiteFen, whiteInvalidMoves, true, 0);
    testMoves(blackFen, blackInvalidMoves, true, 0);
}

TEST_F(MoveValidationTests, PawnMoves){
    std::string whiteFen = "k7/pp2PP1p/6p1/1NpP4/5r2/2P1P2b/P1P3P1/K7 w - c6 0 2";
    std::string blackFen = "k7/p3p3/2pP4/1N6/4Pp1b/2P5/PKP3Pp/8 b - e3 0 1";

    std::list<Move> whiteRegularMoves;
    std::list<Move> whiteDoubleMoves;
    std::list<Move> whiteEnPassantMoves;
    std::list<Move> whitePromotionMoves;

    whiteRegularMoves.push_back(Move(Not2Ind("a2"), Not2Ind("a3"), 0));
    whiteRegularMoves.push_back(Move(Not2Ind("c3"), Not2Ind("c4"), 0));
    whiteRegularMoves.push_back(Move(Not2Ind("d5"), Not2Ind("d6"), 0));
    whiteRegularMoves.push_back(Move(Not2Ind("e3"), Not2Ind("e4"), 0));
    whiteRegularMoves.push_back(Move(Not2Ind("g2"), Not2Ind("g3"), 0));

    whiteDoubleMoves.push_back(Move(Not2Ind("a2"), Not2Ind("a4"), 0));
    whiteDoubleMoves.push_back(Move(Not2Ind("g2"), Not2Ind("g4"), 0));

    whiteEnPassantMoves.push_back(Move(Not2Ind("d5"), Not2Ind("c6"), 0));

    whitePromotionMoves.push_back(Move(Not2Ind("e7"), Not2Ind("e8"), 0, WHITE_QUEEN));
    whitePromotionMoves.push_back(Move(Not2Ind("e7"), Not2Ind("e8"), 0, WHITE_BISHOP));

    std::list<Move> blackRegularMoves;
    std::list<Move> blackDoubleMoves;
    std::list<Move> blackEnPassantMoves;
    std::list<Move> blackPromotionMoves;

    blackRegularMoves.push_back(Move(Not2Ind("a7"), Not2Ind("a6"), 0));
    blackRegularMoves.push_back(Move(Not2Ind("c6"), Not2Ind("c5"), 0));
    blackRegularMoves.push_back(Move(Not2Ind("e7"), Not2Ind("e6"), 0));
    blackRegularMoves.push_back(Move(Not2Ind("a7"), Not2Ind("a6"), 0));

    blackDoubleMoves.push_back(Move(Not2Ind("a7"), Not2Ind("a5"), 0));
    blackDoubleMoves.push_back(Move(Not2Ind("e7"), Not2Ind("e5"), 0));

    blackEnPassantMoves.push_back(Move(Not2Ind("f4"), Not2Ind("e3"), 0));

    blackPromotionMoves.push_back(Move(Not2Ind("h2"), Not2Ind("h1"), 0, BLACK_ROOK));


    testMoves(whiteFen, whiteRegularMoves, false, PAWN_MOVE);
    testMoves(whiteFen, whiteDoubleMoves, false, PAWN_MOVE | PAWN_DOUBLE_MOVE);
    testMoves(whiteFen, whiteEnPassantMoves, false, PAWN_MOVE | CAPTURE_MOVE | EN_PASSANT_MOVE);
    testMoves(whiteFen, whitePromotionMoves, false, PAWN_MOVE | PROMOTION_MOVE);

    testMoves(blackFen, blackRegularMoves, false, PAWN_MOVE);
    testMoves(blackFen, blackDoubleMoves, false, PAWN_MOVE | PAWN_DOUBLE_MOVE);
    testMoves(blackFen, blackEnPassantMoves, false, PAWN_MOVE | CAPTURE_MOVE | EN_PASSANT_MOVE);
    testMoves(blackFen, blackPromotionMoves, false, PAWN_MOVE | PROMOTION_MOVE);
}


TEST_F(MoveValidationTests, Castling){
    std::string whiteCastleOK = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";
    std::string blackCastleOK = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1";

    std::list<Move> whiteCastles;
    std::list<Move> blackCastles;
    whiteCastles.push_back(Move(Not2Ind("e1"), Not2Ind("g1"), 0));
    whiteCastles.push_back(Move(Not2Ind("e1"), Not2Ind("c1"), 0));

    blackCastles.push_back(Move(Not2Ind("e8"), Not2Ind("g8"), 0));
    blackCastles.push_back(Move(Not2Ind("e8"), Not2Ind("c8"), 0));

    // check castling for white
    std::stringstream sstr(whiteCastleOK);
    std::unique_ptr<Position> position(new Position(sstr));
    auto moveIterator = whiteCastles.begin();
    std::unique_ptr<Move> result = std::unique_ptr<Move>(position->CheckIfMoveFullLegal(&(*moveIterator)));
    ASSERT_NE(result.get(), nullptr);
    EXPECT_EQ(result->Type(), SHORT_CASTLE_MOVE);
    
    ++moveIterator;
    result = std::unique_ptr<Move>(position->CheckIfMoveFullLegal(&(*moveIterator)));
    ASSERT_NE(result.get(), nullptr);
    EXPECT_EQ(result->Type(), LONG_CASTLE_MOVE);

    // check castling for black
    sstr = std::stringstream(blackCastleOK);
    position = std::unique_ptr<Position>(new Position(sstr));
    moveIterator = blackCastles.begin();
    result = std::unique_ptr<Move>(position->CheckIfMoveFullLegal(&(*moveIterator)));
    ASSERT_NE(result.get(), nullptr);
    EXPECT_EQ(result->Type(), SHORT_CASTLE_MOVE);

    ++moveIterator;
    result = std::unique_ptr<Move>(position->CheckIfMoveFullLegal(&(*moveIterator)));
    ASSERT_NE(result.get(), nullptr);
    EXPECT_EQ(result->Type(), LONG_CASTLE_MOVE);
}


TEST_F(MoveValidationTests, CastlingForbidden){
    std::string whiteBlockedCastle = "r3k2r/ppp1p1pp/3R4/3B4/3b4/3r4/PPP1P1PP/R3K2R w KQkq - 0 1";
    std::string blackBlockedCastle = "r3k2r/ppp1p1pp/3R4/3B4/3b4/3r4/PPP1P1PP/R3K2R b KQkq - 0 1";
    std::string whiteNoPermission = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w - - 4 3";
    std::string blackNoPermission = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b - - 4 3";
    std::string whitePiecesInBetween = "r1p1k1nr/pppppppp/8/8/8/8/PPPPPPPP/R1R1KN1R w KQkq - 0 1";
    std::string blackPiecesInBetween = "r1p1k1nr/pppppppp/8/8/8/8/PPPPPPPP/R1R1KN1R b KQkq - 0 1";
    std::string whiteKingChecked = "r3k2r/ppp1pppp/3p4/1B6/7q/5P2/PPPPP1PP/R3K2R w KQkq - 0 1";
    std::string blackKingChecked = "r3k2r/ppp1pppp/3p4/1B6/7q/5P2/PPPPP1PP/R3K2R b KQkq - 0 1";


    std::list<Move> whiteCastles;
    std::list<Move> blackCastles;
    whiteCastles.push_back(Move(Not2Ind("e1"), Not2Ind("g1"), 0));
    whiteCastles.push_back(Move(Not2Ind("e1"), Not2Ind("c1"), 0));

    blackCastles.push_back(Move(Not2Ind("e8"), Not2Ind("g8"), 0));
    blackCastles.push_back(Move(Not2Ind("e8"), Not2Ind("c8"), 0));

    testMoves(whiteBlockedCastle, whiteCastles, true, 0);
    testMoves(blackBlockedCastle, blackCastles, true, 0);
    testMoves(whiteNoPermission, whiteCastles, true, 0);
    testMoves(blackNoPermission, blackCastles, true, 0);
    testMoves(whitePiecesInBetween, whiteCastles, true, 0);
    testMoves(blackPiecesInBetween, blackCastles, true, 0);
    testMoves(whiteKingChecked, whiteCastles, true, 0);
    testMoves(blackKingChecked, blackCastles, true, 0);
}

TEST_F(MoveValidationTests, LeavingOwnKingUnderCheck){
    std::string whiteFen = "3q4/5k2/8/1K1Pp2r/6r1/3B4/8/5b2 w - - 0 2";
    std::string blackFen = "1r6/5k2/8/8/5n2/1B6/8/K3R3 b - - 0 1";

    std::list<Move> whiteInvalidMoves;
    std::list<Move> blackValidMoves;
    std::list<Move> blackInvalidMoves;

    // moving king to attacked place
    whiteInvalidMoves.push_back(Move(Not2Ind("b5"), Not2Ind("b6"), 0));
    whiteInvalidMoves.push_back(Move(Not2Ind("b5"), Not2Ind("a5"), 0));
    whiteInvalidMoves.push_back(Move(Not2Ind("b5"), Not2Ind("b4"), 0));
    whiteInvalidMoves.push_back(Move(Not2Ind("b5"), Not2Ind("a4"), 0));

    // leaving king under attack by moving other piece
    whiteInvalidMoves.push_back(Move(Not2Ind("d3"), Not2Ind("c2"), 0));
    whiteInvalidMoves.push_back(Move(Not2Ind("d3"), Not2Ind("e4"), 0));

    // leaving king under attack after en passant move
    whiteInvalidMoves.push_back(Move(Not2Ind("d5"), Not2Ind("e6"), 0));


    blackValidMoves.push_back(Move(Not2Ind("f4"), Not2Ind("e6"), 0));
    blackValidMoves.push_back(Move(Not2Ind("f4"), Not2Ind("d5"), 0));
    blackValidMoves.push_back(Move(Not2Ind("b8"), Not2Ind("b3"), 0));
    blackValidMoves.push_back(Move(Not2Ind("f7"), Not2Ind("f6"), 0));
    blackValidMoves.push_back(Move(Not2Ind("f7"), Not2Ind("g7"), 0));

    blackInvalidMoves.push_back(Move(Not2Ind("f4"), Not2Ind("c4"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("f7"), Not2Ind("e6"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("f7"), Not2Ind("e7"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("f7"), Not2Ind("g8"), 0));
    blackInvalidMoves.push_back(Move(Not2Ind("b8"), Not2Ind("e8"), 0));

    testMoves(whiteFen, whiteInvalidMoves, true, 0);
    testMoves(blackFen, blackValidMoves, false, 0, false);
    testMoves(blackFen, blackInvalidMoves, true, 0);
}
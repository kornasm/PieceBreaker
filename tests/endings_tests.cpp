#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../src/functions.h"
#include "../src/node.h"
#include "../src/position.h"
#include "../src/move.h"

void testGameResult(std::list<std::string> fenList, GameResult result){

    for(auto fen : fenList){
        std::stringstream sstr(fen);
        std::unique_ptr<Position> position(new Position(sstr));
        EXPECT_EQ(position->GetGameResult(), result) << "  Wrong result.  Fen is  " << fen << std::endl;
    }
}


TEST(EndingsTests, Checkmate){
    Init();

    std::list<std::string> blackWins;
    std::list<std::string> whiteWins;

    blackWins.push_back("3K2q1/8/3k4/8/8/8/8/8 w - - 0 1");
    blackWins.push_back("8/6q1/8/5K2/3r4/k6b/8/8 w - - 0 1");
    blackWins.push_back("8/4k3/8/3B4/8/7b/2PP2R1/1Q1K1q2 w - - 0 1");
    blackWins.push_back("8/8/6b1/8/k7/2P5/1P1PP3/2K3r1 w - - 0 1");
    blackWins.push_back("r7/4k3/6b1/4P3/3P4/2P5/1Pq5/1K6 w - - 0 1");
    blackWins.push_back("rnb1kbnr/pppp1ppp/4p3/8/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 0 1");

    whiteWins.push_back("3k4/7R/8/8/B5Q1/3R4/3K4/8 b - - 0 1");
    whiteWins.push_back("7R/4k3/R5N1/8/8/3R4/3K4/5R2 b - - 0 1");
    whiteWins.push_back("1n1qkn2/pppprb1p/5N2/8/8/8/RN3PPP/5RK1 b - - 0 1");

    testGameResult(whiteWins, GameResult::WHITE_WIN);
    testGameResult(blackWins, GameResult::BLACK_WIN);

    Cleanup();
}

TEST(EndingsTests, Stalemate){
    Init();
    std::list<std::string> stalemateFens;

    stalemateFens.push_back("3k4/8/2Q1K3/8/8/8/8/8 b - - 0 1");
    stalemateFens.push_back("8/2R5/5p1k/p4P2/Pp5K/1P6/8/8 b - - 0 1");
    stalemateFens.push_back("8/3q3n/7P/4K3/6r1/1k3pP1/1P3P2/7N w - - 0 1");

    testGameResult(stalemateFens, GameResult::DRAW);
    Cleanup();
}

TEST(EndingsTest, 50MoveRule){
    Init();
    std::string fen = "8/1k3r2/8/6n1/8/2K5/4R3/8 w - - 99 5";

    std::stringstream sstr(fen);

    std::unique_ptr<Position> position1(new Position(sstr));

    ASSERT_EQ(position1->GetGameResult(), GameResult::ONGOING);

    //Move *move = new Move(Not2Ind("e2"), Not2Ind("d2"), 0);

    std::unique_ptr<Move> move(new Move(Not2Ind("e2"), Not2Ind("d2"), 0));
    std::unique_ptr<Position> position2(new Position(*(position1.get()), move.get()));

    ASSERT_NE(position2.get(), nullptr);
    EXPECT_EQ(position2->GetGameResult(), GameResult::DRAW);
    Cleanup();
}

TEST(EndingsTests, RepetitionDraw){
    Init();

    std::string fen = "6k1/6r1/2ppp3/1qq1qp1Q/1qq5/8/8/7K w - - 0 1";
    std::stringstream sstr(fen);

    std::unique_ptr<Move> move1(new Move(Not2Ind("h5"), Not2Ind("e8"), 0));
    std::unique_ptr<Move> move3(new Move(Not2Ind("e8"), Not2Ind("h5"), 0));
    std::unique_ptr<Move> move4(new Move(Not2Ind("h7"), Not2Ind("g8"), 0));
    std::unique_ptr<Move> move2(new Move(Not2Ind("g8"), Not2Ind("h7"), 0));

    std::unique_ptr<Position> position1(new Position(sstr));
    std::unique_ptr<Position> position2(new Position(*(position1.get()), move1.get()));
    std::unique_ptr<Position> position3(new Position(*(position2.get()), move2.get()));
    std::unique_ptr<Position> position4(new Position(*(position3.get()), move3.get()));
    std::unique_ptr<Position> position5(new Position(*(position4.get()), move4.get()));
    std::unique_ptr<Position> position6(new Position(*(position5.get()), move1.get()));
    std::unique_ptr<Position> position7(new Position(*(position6.get()), move2.get()));
    std::unique_ptr<Position> position8(new Position(*(position7.get()), move3.get()));
    std::unique_ptr<Position> position9(new Position(*(position8.get()), move4.get()));
    EXPECT_EQ(position8->GetGameResult(), GameResult::ONGOING);
    EXPECT_EQ(position9->GetGameResult(), GameResult::DRAW);
    Cleanup();
}

TEST(EndingsTests, InsufficientMaterial){
    Init();
    
    std::list<std::string> drawFens;
    std::list<std::string> notDrawFens;

    drawFens.push_back("");
    drawFens.push_back("");
    drawFens.push_back("");
    drawFens.push_back("");

    notDrawFens.push_back("");
    notDrawFens.push_back("");
    notDrawFens.push_back("");
    notDrawFens.push_back("");
    notDrawFens.push_back("");

    Cleanup();
}
#include <gtest/gtest.h>

#include "move.h"
#include "declarations.h"
#include "board.h"

#include <array>
#include <string>
#include <memory>

TEST(MoveTests, GettersReturnSameValueAsSaved){
    // some of the values in type and promo arrays
    // the test validates just data read is the same as saved
    constexpr int arraySize = 10;
    std::array<int, arraySize> fromArray = {43, 65, 22, 75, 34, 59, 42, 22, 92, 29};
    std::array<int, arraySize> toArray = {47, 33, 22, 55, 53, 56, 32, 99, 37, 49};
    
    std::array<int, arraySize> typeArray = {4, 2, 1, 8, 34, 59, 64, 16, 53, 16};
    std::array<int, arraySize> promoArray = {1, 3, 5, 7, 2, 0, 3, 7, 8, 0};

    for(int i = 0; i < arraySize; i++){
        Move m = Move(fromArray[i], toArray[i], typeArray[i], promoArray[i]);
        EXPECT_EQ(m.From(), fromArray[i]);
        EXPECT_EQ(m.To(), toArray[i]);
        EXPECT_EQ(m.Type(), typeArray[i]);
        EXPECT_EQ(m.Promo(), promoArray[i]);
    }
}

TEST(MoveTests, IncreateType){
    constexpr int arraySize = 10;
    std::array<int, arraySize> baseTypes = {43, 24, 22, 2, 8, 59, 0, 32, 0, 32};
    std::array<int, arraySize> increaseTypes = {1, 4, 8, 12, 24, 18, 9, 64, 37, 49};
    for(int i = 0; i < arraySize; i++){
        Move m = Move(0, 0, baseTypes[i], 0);

        m.IncreaseType(increaseTypes[i]);

        EXPECT_EQ(m.Type(), baseTypes[i] | increaseTypes[i]);
    }
}

TEST(MoveTests, ReadingMoves){
    constexpr int arraySize = 10;
    std::array<std::string, arraySize> notations{
        "e2a5",
        "a7e8q",
        "b4b2",
        "d2d1r",
        "e3c8",
        "d7h6",
        "g6d4",
        "h7h8b",
        "e2d1n",
        "c8h3",
    };
    std::array<Move, arraySize> moves = {
        Move(Board::Not2Ind("e2"), Board::Not2Ind("a5"), 0, 0),
        Move(Board::Not2Ind("a7"), Board::Not2Ind("e8"), 0, WHITE_QUEEN),
        Move(Board::Not2Ind("b4"), Board::Not2Ind("b2"), 0, 0),
        Move(Board::Not2Ind("d2"), Board::Not2Ind("d1"), 0, WHITE_ROOK),
        Move(Board::Not2Ind("e3"), Board::Not2Ind("c8"), 0, 0),
        Move(Board::Not2Ind("d7"), Board::Not2Ind("h6"), 0, 0),
        Move(Board::Not2Ind("g6"), Board::Not2Ind("d4"), 0, 0),
        Move(Board::Not2Ind("h7"), Board::Not2Ind("h8"), 0, WHITE_BISHOP),
        Move(Board::Not2Ind("e2"), Board::Not2Ind("d1"), 0, WHITE_KNIGHT),
        Move(Board::Not2Ind("c8"), Board::Not2Ind("h3"), 0, 0)
    };
    

    for(int i = 0; i < arraySize; i++){
        std::unique_ptr<Move> m(Move::String2Move(notations[i]));
        EXPECT_EQ(moves[i].From(), m->From());
        EXPECT_EQ(moves[i].To(), m->To());
        EXPECT_EQ(moves[i].Type(), m->Type());
        EXPECT_EQ(moves[i].Promo(), m->Promo());
    }
}

TEST(MoveTests, PrintingMoves){
    constexpr int arraySize = 10;
    std::array<Move, arraySize> moves = {
        Move(Board::Not2Ind("e2"), Board::Not2Ind("a5"), 0, 0),
        Move(Board::Not2Ind("a7"), Board::Not2Ind("e8"), 0, WHITE_QUEEN),
        Move(Board::Not2Ind("b4"), Board::Not2Ind("b2"), 0, 0),
        Move(Board::Not2Ind("d2"), Board::Not2Ind("d1"), 0, BLACK_ROOK),
        Move(Board::Not2Ind("e3"), Board::Not2Ind("c8"), 0, 0),
        Move(Board::Not2Ind("d7"), Board::Not2Ind("h6"), 0, 0),
        Move(Board::Not2Ind("g6"), Board::Not2Ind("d4"), 0, 0),
        Move(Board::Not2Ind("h7"), Board::Not2Ind("h8"), 0, WHITE_BISHOP),
        Move(Board::Not2Ind("e2"), Board::Not2Ind("d1"), 0, BLACK_KNIGHT),
        Move(Board::Not2Ind("c8"), Board::Not2Ind("h3"), 0, 0)
    };
    std::array<std::string, arraySize> notations{
        "e2a5",
        "a7e8q",
        "b4b2",
        "d2d1r",
        "e3c8",
        "d7h6",
        "g6d4",
        "h7h8b",
        "e2d1n",
        "c8h3",
    };

    for(int i = 0; i < arraySize; i++){
        EXPECT_EQ(moves[i].ToString(), notations[i]);
    }
}
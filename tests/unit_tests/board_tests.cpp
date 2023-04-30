#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "board.h"
#include "movegenerators.h"
#include "movecheck.h"
#include "node.h"
#include "move.h"

class BoardTests: public ::testing::Test {
    protected:
        BoardTests() {
            MoveCheckHandler::Init();
            MoveGeneratorHandler::Init();
        }
        ~BoardTests() override {
           MoveCheckHandler::Cleanup();
           MoveGeneratorHandler::Cleanup();
        }
};


TEST_F(BoardTests, BoardIndiciesConverting){
    std::string notation = "a1";
    std::vector<int> rows = {1, 5, 4, 8, 1, 2, 3, 4, 6, 1, 8};
    std::vector<int> columns = {1, 1, 2, 2, 3, 3, 3, 5, 6, 8, 8};
    std::vector<int> indicies = {22, 62, 53, 93, 24, 34, 44, 56, 77, 29, 99};
    std::vector<std::string> notations = {"a1", "a5", "b4", "b8", "c1", "c2", "c3", "e4", "f6", "h1", "h8"};
    for(uint i = 0; i < indicies.size(); i++){
        EXPECT_EQ(Board::Not2Ind(notations[i]), indicies[i]);
        EXPECT_EQ(Board::Ind2Not(indicies[i]), notations[i]);
        EXPECT_EQ(columns[i], Board::column(indicies[i]));
        EXPECT_EQ(rows[i], Board::row(indicies[i]));
    }
}

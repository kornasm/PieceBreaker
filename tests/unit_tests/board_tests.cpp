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

TEST_F(BoardTests, FenReadingPrinting){
    std::vector<std::string> fens = { "rn3bn1/pbppk1pp/1p2p2r/2q1Np2/8/2P1PK2/P1PP1PPP/RNBQ1B1R w - - 0 1",
                                      "4k3/Q7/4K3/8/8/8/8/8 w - - 0 1",
                                      "4k3/Q7/4K3/8/8/8/8/8 b - - 10 115",
                                      "rnbqkb1r/pp3ppp/3p1n2/2pPp3/4P3/5N2/PPP2PPP/RNBQKB1R w KQkq c6 38 75" };
    for(auto inFen : fens){
        std::stringstream sstr(inFen);
        Node *nd = new Node(sstr);
        std::string outFen = nd->GetFen();
        EXPECT_EQ(inFen, outFen) << "Test:  " << inFen << "  failed\n";
        //std::cerr << "#" << inFen << "#\n#" << outFen << "#\n";
        delete nd;
    }
}

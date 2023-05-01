#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <list>

#include "move.h"
#include "movecheck.h"
#include "movegenerators.h"
#include "movecheck.h"
#include "position.h"

class MoveGenerationTests: public ::testing::Test {
    protected:
        MoveGenerationTests() {
            MoveCheckHandler::Init();
            MoveGeneratorHandler::Init();
        }
        ~MoveGenerationTests() override {
           MoveCheckHandler::Cleanup();
           MoveGeneratorHandler::Cleanup();
        }
};

TEST_F(MoveGenerationTests, PseudoLegalMoves){
    std::array<std::string, 4> fens = {
        std::string("4k3/2pr4/8/q4pP1/8/2PP4/5P2/R3KB1R w KQ f6 0 2"),
        std::string("4k3/2pr4/8/q4pP1/8/2PP4/5P2/R3KB1R b KQ - 0 2"),
        std::string("4k3/4qr2/8/b7/1N2N2b/4r1N1/8/R3K2R w - - 0 1"),
        std::string("2bk3r/5N1r/5p2/8/7B/3n3Q/8/1K1RR3 b - - 0 1")
    };
    std::array<int, 4> noMoves = {
        28,
        30,
        34,
        28
    };
    std::stringstream fen(std::string("4k3/2pr4/8/q4pP1/8/2PP4/5P2/R3KB1R w KQ f6 0 2"));
    
    
    for(unsigned int i = 0; i < fens.size(); i++){
        std::stringstream sstr(fens[i]);
        std::unique_ptr<Position> pos(new Position(sstr));
        std::list<Move> moves = AllMovesGenerator::GenerateMoves(*pos.get());
        int success = 1;
        EXPECT_EQ(moves.size(), noMoves[i]) << success-- << "\n";
        for(auto move: moves){
            // apart from list size each move is checked whether it's valid
            std::unique_ptr<Move> mov(MoveCheckHandler::CheckMove(*pos.get(), move.From(), move.To()));
            EXPECT_NE(mov.get(), nullptr) << "generated move " << move << " is not valid";
        }
        if(success == 0){
           for(Move move: moves){
                std::cout << move << "\n";
            }
        }
    }
}


TEST_F(MoveGenerationTests, FullLegalMoves){
        std::array<std::string, 4> fens = {
        std::string("4k3/2pr4/8/q4pP1/8/2PP4/5P2/R3KB1R w KQ f6 0 2"),
        std::string("4k3/2pr4/8/q4pP1/8/2PP4/5P2/R3KB1R b KQ - 0 2"),
        std::string("4k3/4qr2/8/b7/1N2N2b/4r1N1/8/R3K2R w - - 0 1"),
        std::string("2bk3r/5N1r/5p2/8/7B/3n3Q/8/1K1RR3 b - - 0 1")
    };
    std::array<int, 4> noMoves = {
        27,
        30,
        2,
        2
    };
    std::stringstream fen(std::string("4k3/2pr4/8/q4pP1/8/2PP4/5P2/R3KB1R w KQ f6 0 2"));
    
    
    for(unsigned int i = 0; i < fens.size(); i++){
        std::stringstream sstr(fens[i]);
        std::unique_ptr<Position> pos(new Position(sstr));
        std::list<Move> moves = pos->GenerateAllLegalMoves();
        int success = 1;
        EXPECT_EQ(moves.size(), noMoves[i]) << fens[i] << "  " << success-- << "\n";
        // individul moves are not checked, because it's already checked in GenerateAllLegalMoves func
        if(success == 0){
           for(Move move: moves){
                std::cout << move << "\n";
            }
        }
    }
}
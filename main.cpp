#include "src/position.h"
#include "src/movegenerators.h"
#include "src/functions.h"
#include "src/node.h"
#include "src/move.h"
#include "src/search.h"
#include "src/evaluate.h"
#include "src/uci.h"

int main(int argc, char **argv){
    PieceBreaker::Init(argc, argv);
    Uci::loop();
    PieceBreaker::Cleanup();
}
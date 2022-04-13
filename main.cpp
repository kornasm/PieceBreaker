#include "src/position.h"
#include "src/movegenerators.h"
#include "src/functions.h"
#include "src/node.h"
#include "src/move.h"
#include "src/search.h"
#include "src/evaluate.h"
#include "src/uci.h"

int main(int argc, char **argv){
    if(argc == 1){
        freopen("/dev/null", "w", stderr);
    }
    if(argc > 1){
        freopen(argv[1], "r", stdin);
    }
    Init();
    Uci::loop();
    Cleanup();
}
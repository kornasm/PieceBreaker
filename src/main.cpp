#include "position.h"
#include "movegenerators.h"
#include "functions.h"
#include "node.h"
#include "move.h"
#include "search.h"
#include "evaluate.h"
#include "uci.h"

int main(int argc, char **argv){
    if(argc == 1){
        freopen("/dev/null", "w", stderr);
    }
    init();
    Uci::loop();
    cleanup();
}
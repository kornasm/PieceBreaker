#include "position.h"
#include "movegenerators.h"
#include "functions.h"
#include "node.h"
#include "move.h"
#include "search.h"
#include "evaluate.h"
#include "uci.h"

int main(){
    freopen("/dev/null", "w", stderr);
    init();
    Uci::loop();
}
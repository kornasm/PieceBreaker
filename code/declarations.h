#ifndef declarations
#define declarations

#include "node.h"
#include "board.h"
#include "functions.h"
#include "movegenerators.h"
#include "move.h"

#include<string>
#include<iostream>
#include <list>
#include<algorithm>

#include<unistd.h>

#define SYMBOLS_OFFSET 6
#define NO_PIECES 13

// colors
#define OUTSIDE_BOARD 7
#define WHITE 1
#define EMPTY_SQUARE 0
#define BLACK -1


// move types
#define REGULAR_MOVE 0
#define CAPTURE_MOVE 1
#define SHORT_CASTLE_MOVE 2
#define LONG_CASTLE_MOVE 4
#define PAWN_DOUBLE_MOVE 8
#define EN_PASSANT_MOVE 16
#define PROMOTION_MOVE 32

#endif
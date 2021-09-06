#ifndef DECLARATIONS_H_
#define DECLARATIONS_H_

#include "position.h"
#include "functions.h"
#include "movegenerators.h"
#include "move.h"
#include "node.h"

#include<string>
#include<iostream>
#include <list>
#include<algorithm>

#define SYMBOLS_OFFSET 6
#define NO_PIECES 13

// colors
#define OUTSIDE_BOARD 7
#define WHITE 1
#define EMPTY_SQUARE 0
#define BLACK -1

// pieces
#define BLACK_KING -6
#define BLACK_QUEEN -5
#define BLACK_BISHOP -4
#define BLACK_KNIGHT -3
#define BLACK_ROOK -2
#define BLACK_PAWN -1
#define WHITE_PAWN 1
#define WHITE_ROOK 2
#define WHITE_KNIGHT 3
#define WHITE_BISHOP 4
#define WHITE_QUEEN 5
#define WHITE_KING 6

// move types
#define REGULAR_MOVE 0
#define SHORT_CASTLE_MOVE 1
#define LONG_CASTLE_MOVE 2
#define PAWN_MOVE 4
#define PAWN_DOUBLE_MOVE 8
#define CAPTURE_MOVE 16
#define EN_PASSANT_MOVE 32
#define PROMOTION_MOVE 64

#endif
## Overview

PieceBreaker is simple chess engine. It does not provide GUI. Full support for the [the UCI protocol](https://www.shredderchess.com/download/div/uci.zip) will be implemented soon.


## Compiling
Best to use with g++ (other compilers not tested). CMake required.

'''
    cmake -B build/
    cmake --build build/
    build/PieceBreaker
'''

## Usage
Some commands from UCI protocol are available. Most important are listed below:

  * #### position fen [fen]
    Set position to be evaluated. Fen string can be obtained [here](https://lichess.org/editor).

  * #### go [number]
    Start searching to the depth passed to [number].

  * #### stop
    Stop searching and print best move fount.

  * #### quit
    Quit the program.
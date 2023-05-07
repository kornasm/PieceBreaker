# Overview

PieceBreaker is a terminal based simple chess engine. Communication with GUI will be possible via [the UCI protocol](https://www.shredderchess.com/download/div/uci.zip). Full support will be implemented soon.


## Compiling
Best to use with g++ (other compilers not tested). CMake required.

```
    cmake -B build/
    cmake --build build/
```

## Running
```
    ./build/PieceBreaker
```

## Usage
Some commands from UCI protocol are already available. Most important are listed below:

  * #### position [fen <fenstring> | startpos] moves \<move 1\> ... \<move n\>
    Set position to be analyzed. "Moves" part is optional. You can obtain fen string on [lichess board editor](https://lichess.org/editor).

  * #### go \<number\>
    Start searching to the depth passed to \<number\>.

  * #### stop
    Stop searching and print best move found.

  * #### quit
    Quit the program.
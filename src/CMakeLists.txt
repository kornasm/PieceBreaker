set(CMAKE_CXX_FLAGS_INIT "-std=c++17 -Wall -Wshadow -Wextra -Wno-unused-parameter -Wconversion")

set(This libpcbr)

set(Sources
    movegenerators.cpp move.cpp evaluate.cpp functions.cpp movecheck.cpp node.cpp position.cpp search.cpp uci.cpp
)

set(Headers
    movegenerators.h move.h evaluate.h functions.h movecheck.h node.h position.h search.h uci.h
)
add_library(${This} ${Sources} ${Headers})

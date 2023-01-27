
set(This libpcbr)

set(Sources
    movegenerators.cpp move.cpp evaluate.cpp functions.cpp movecheck.cpp node.cpp position.cpp search.cpp uci.cpp input_provider.cpp
)

set(Headers
    movegenerators.h move.h evaluate.h functions.h movecheck.h node.h position.h search.h uci.h input_provider.h
)
add_library(${This} ${Sources} ${Headers})

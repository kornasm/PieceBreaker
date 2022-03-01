cmake_minimum_required(VERSION 3.10)

project(PieceBreaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wshadow -Wextra -Wno-unused-parameter -Wconversion")

add_subdirectory(src)
add_executable(PieceBreaker main.cpp)

target_link_libraries(PieceBreaker libpcbr)
target_link_libraries(PieceBreaker pthread)
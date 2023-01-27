cmake_minimum_required(VERSION 3.10)

set(This PieceBreaker)
project(${This} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wshadow -Wextra -Wno-unused-parameter -Wconversion --coverage")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)

add_executable(${This} main.cpp)
target_link_libraries(${This} libpcbr)
target_link_libraries(${This} pthread)
target_link_libraries(${This} boost_program_options)
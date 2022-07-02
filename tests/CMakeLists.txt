cmake_minimum_required(VERSION 3.10)

set(This PieceBreakerTest)
set(Sources test.cpp)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)

add_executable(board_tests board_tests.cpp)
add_executable(move_validation_tests move_validation_tests.cpp)

target_link_libraries(GTest::GTest INTERFACE gtest_main)

target_link_libraries(board_tests PRIVATE GTest::GTest libpcbr)
target_link_libraries(move_validation_tests PRIVATE GTest::GTest libpcbr)

enable_testing()

add_test(PCB_board_tests board_tests)
add_test(PCB_move_validation_tests move_validation_tests)
cmake_minimum_required(VERSION 3.10)

include_directories("../../src")
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
add_executable(move_tests move_tests.cpp)
add_executable(move_generation_tests move_generation.cpp)
add_executable(move_validation_tests move_validation_tests.cpp)
add_executable(endings_tests endings_tests.cpp)

target_link_libraries(GTest::GTest INTERFACE gtest_main)

target_link_libraries(board_tests PRIVATE GTest::GTest libpcbr boost_program_options)
target_link_libraries(move_tests PRIVATE GTest::GTest libpcbr boost_program_options)
target_link_libraries(move_generation_tests PRIVATE GTest::GTest libpcbr boost_program_options)
target_link_libraries(move_validation_tests PRIVATE GTest::GTest libpcbr boost_program_options)
target_link_libraries(endings_tests PRIVATE GTest::GTest libpcbr boost_program_options)

enable_testing()

add_test(PCBR_board_tests board_tests)
add_test(PBCR_move_tests move_tests)
add_test(PCBR_move_generation_tests move_generation_tests)
add_test(PCBR_move_validation_tests move_validation_tests)
add_test(PBCR_endings_tests endings_tests)
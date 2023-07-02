cmake_minimum_required(VERSION 3.22)
project(sudoku)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        sudoku
        src/main.cpp
        src/sudoku.cpp
)

# Tests
enable_testing()

add_test(
        NAME generate_final_board
        COMMAND $<TARGET_FILE:sudoku> -c 100
)

add_test(
        NAME generate_sudoku_game
        COMMAND $<TARGET_FILE:sudoku> -n 100
)

add_test(
        NAME generate_sudoku_game_easy
        COMMAND $<TARGET_FILE:sudoku> -n 100 -m 1
)

add_test(
        NAME generate_sudoku_game_normal
        COMMAND $<TARGET_FILE:sudoku> -n 100 -m 2
)

add_test(
        NAME generate_sudoku_game_diffcult
        COMMAND $<TARGET_FILE:sudoku> -n 100 -m 3
)

add_test(
        NAME generate_sudoku_game_range
        COMMAND $<TARGET_FILE:sudoku> -n 100 -r 20~30
)

add_test(
        NAME generate_sudoku_game_unique
        COMMAND $<TARGET_FILE:sudoku> -n 100 -u
)

add_test(
        NAME solve_game
        COMMAND $<TARGET_FILE:sudoku> -s ${CMAKE_CURRENT_SOURCE_DIR}/test/test_solve.txt
)

add_test(
        NAME test_error_input
        COMMAND $<TARGET_FILE:sudoku> -s ${CMAKE_CURRENT_SOURCE_DIR}/test/test_error.txt
)

add_test(
        NAME test_file_not_exist
        COMMAND $<TARGET_FILE:sudoku> -s ${CMAKE_CURRENT_SOURCE_DIR}/test/not_exist.txt
)

add_test(
        NAME test_error_param
        COMMAND $<TARGET_FILE:sudoku> -x
)

add_test(
        NAME test_error_num
        COMMAND $<TARGET_FILE:sudoku> -c 1000001
)

add_test(
        NAME test_error_mode
        COMMAND $<TARGET_FILE:sudoku> -n 10 -m 4
)

add_test(
        NAME test_error_range
        COMMAND $<TARGET_FILE:sudoku> -n 100 -r 40~60
)

add_test(
        NAME test_error_too_many_params
        COMMAND $<TARGET_FILE:sudoku> -m 2 -r 30~40 -u
)

set_tests_properties(
        test_error_input
        test_file_not_exist
        test_error_param
        test_error_num
        test_error_mode
        test_error_range
        test_error_too_many_params
        PROPERTIES WILL_FAIL TRUE
)

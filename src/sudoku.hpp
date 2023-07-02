#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <iostream>

#include <string>
#include <vector>
#include <array>

typedef std::array<std::array<int, 9>, 9> board_t;
typedef std::array<std::array<int, 9>, 9> constraint_t;
typedef std::pair<int, int> range_t;

std::vector<board_t> read_boards(const std::string &filename);
void write_boards(const std::string &filename, const std::vector<board_t> &boards);
void print_board(const board_t &board);

std::vector<board_t> generate_final_boards(int count);
std::vector<board_t> generate_sudoku_games(int count, int mode, bool unique, range_t range);
board_t solve_board(const board_t &board);

#endif
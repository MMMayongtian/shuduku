#include <fstream>
#include <sstream>

#include <algorithm> // std::move_backward
#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock

#include "sudoku.hpp"

int uniqueflag = 0;
constraint_t rows, cols, blocks;

range_t mode_map[4] = {range_t(25,55), 
                             range_t(25,35),
                             range_t(35,45),
                             range_t(45,55)};

int cpp_rand(int first, int second) {
    std::random_device seed;//硬件生成随机数种子
    std::ranlux48 engine(seed());//利用种子生成随机数引擎
    std::uniform_int_distribution<> distrib(first, second);//设置随机数范围，并为均匀分布
    return distrib(engine);
}

std::vector<board_t> read_boards(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error("error, no such file!");
    }

    std::vector<board_t> boards;

    std::string line;
    while (std::getline(fin, line) && !line.empty()) {
        std::stringstream linein(line);

        board_t board;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                char v{};
                linein >> v;
                if (!(v >= '1' && v <= '9' || v == '$')) {
                    throw std::runtime_error("Formatting error!");
                }
                if (v == '$') {
                    v = '0';
                }
                board[i][j] = v - '0';
            }
        }
        boards.emplace_back(board);
    }

    return boards;
}

void write_boards(const std::string& filename, const std::vector<board_t>& boards) {
    std::ofstream fout(filename);
    for (const board_t& board : boards) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                char v = board[i][j] == 0 ? '$' : board[i][j] + '0';
                fout << v;
            }
        }
        fout << std::endl;
    }
}

void print_board(const board_t& board) {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            std::cout << "+-------+-------+-------+" << std::endl;
        }
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0) {
                std::cout << "| ";
            }
            std::cout << static_cast<char> (board[i][j] == 0 ?
                ' ' : board[i][j] + '0')
                << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "+-------+-------+-------+" << std::endl;
}

board_t generate_base_board() {
    board_t board;
    const int line[9] = { 3, 1, 4, 2, 7, 9, 6, 5, 8 };
    const int shift[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = line[(j + shift[i]) % 9];
        }
    }

    return board;
}

std::vector<board_t> generate_final_boards(int count) {
    int shift_t[9] = { 3, 1, 4, 2, 7, 9, 6, 5, 8 };
    std::vector<int> shift(std::begin(shift_t), std::end(shift_t));
    board_t base_board = generate_base_board();
    std::vector<board_t> result;
    int n = count;

    do {
        board_t new_board;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(shift.begin(), shift.end(), std::default_random_engine(seed));
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                new_board[i][j] = shift[base_board[i][j] - 1];
            }
        }
        result.emplace_back(new_board);
        n--;
    } while (n);
    return result;
}

void constraint_solve(int i, int j, bool cons[10], const board_t& res) {
    // 排除本行已有的数字
    for (int k = 0; k < 9; k++) {
        if (k == j || res[i][k] == 0)continue;
        cons[res[i][k]] = 1;
    }

    // 排除本列已有的数字
    for (int k = 0; k < 9; k++) {
        if (k == i || res[k][j] == 0)continue;
        cons[res[k][j]] = 1;
    }
    // 九宫格约束
    int n, m;
    if (i / 3 == 0)n = 0;
    else if (i / 6 == 0)n = 3;
    else
        n = 6;

    if (j / 3 == 0)m = 0;
    else if (j / 6 == 0)m = 3;
    else
        m = 6;

    for (int p = n; p < n + 3; p++) {
        for (int q = m; q < m + 3; q++) {
            if ((p == i && q == j) || res[p][q] == 0)continue;
            cons[res[p][q]] = 1;
        }
    }
}

void solve(int i, int j, board_t& res, int solve_num) {

    if (j == 9) {
        i++;
        j = 0;
    }
    if (i == 9) {
        uniqueflag++;
        return;
    }

    if (res[i][j] == 0) {
        bool cons[10] = { false };
        constraint_solve(i, j, cons, res);

        for (int k = 1; k < 10; k++) {
            if (cons[k]) continue;
            res[i][j] = k;
            solve(i, j + 1, res, solve_num);
            if (uniqueflag == solve_num) return;
            res[i][j] = 0;
        }
    }
    else {
        solve(i, j + 1, res, solve_num);
    }
}

board_t solve_board(const board_t& board) {
    uniqueflag = 0;
    board_t res = board;
    solve(0, 0, res, 1);
    return res;
}

board_t dig(board_t board, int hole) {
    // 先每行挖 3 个空
    std::vector<int> blocks_hole;
    for (int i = 0; i < 9; i++) {
        int mid = cpp_rand(0,7);
        blocks_hole.push_back(mid + i * 9);
        blocks_hole.push_back(cpp_rand(0, mid) + i * 9);
        blocks_hole.push_back(cpp_rand(mid + 1, 8) + i * 9);
    }
    for (auto h : blocks_hole) {
        if (hole-- == 0) {
            return board;
        }
        if (board[h / 9][h % 9] != 0) board[h / 9][h % 9] = 0;
        else
            hole++;
    }
    while (hole--) {
        int i = cpp_rand(0, 8);
        int j = cpp_rand(0, 8);
        if (board[i][j] != 0) board[i][j] = 0;
        else
            hole++;
    }
    return board;

}

board_t generate_game_board(board_t board, bool unique, int hole) {
    board_t game_board = dig(board, hole);
    if (unique) {
        
        while (true) {
            uniqueflag = 0;
            board_t res = board;
            solve(0, 0, res, 2);
            if (uniqueflag == 1) return game_board;
            game_board = dig(board, hole);
        }
    }
    else return game_board;
}

std::vector<board_t> generate_sudoku_games(int count, int mode, bool unique, range_t range) {
    std::vector<board_t> result;
    std::vector<board_t> final_boards;

    result.reserve(count);
    final_boards = generate_final_boards(count);

    if (mode != -1) range = mode_map[mode];

    for (int i = 0; i < count; i++) {
        int hole_num = cpp_rand(range.first, range.second);
        result.emplace_back(
            generate_game_board(final_boards[i], unique, hole_num));
    }
    return result;

}

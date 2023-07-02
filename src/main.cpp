#include <iostream>
#include <cstdlib>
#include <ctime>
#include "sudoku.hpp"
using namespace std;

int main(int argc, char* argv[]) {

    if (argc <= 2) {
        cout << "Usage: parameters too few" << endl;
        return 1;
    }

    string option = argv[1];

    if (option == "-c") {
        if (argc != 3) {
            cout << "Usage: sudoku.exe -c <count>" << endl;
            return 1;
        }

        int count = atoi(argv[2]);
        if (count <= 0 || count > 10000) {
            cout << "Invalid count: " << argv[2] << endl;
            return 1;
        }
        // 调用生成函数 数量参数count 
        vector<board_t> boards = generate_final_boards(count);
        for (int i = 0; i < boards.size(); i++) {
            cout<<"Generate the "<<i + 1<<" final boards:"<<endl;
            print_board(boards[i]);
        }
    }
    //绝对或相对路径
    else if (option == "-s") {
        if (argc != 3) {
            cout << "Usage: sudoku.exe -s <filename>" << endl;
            return 1;
        }

        string filename = argv[2];
        //路径参数filename;
        vector<board_t> boards = read_boards(filename);
        for (board_t &board : boards) {
                board = solve_board(board);
                cout << "Solution result:" << endl;
                print_board(board);
        }
        write_boards("sudoku.txt", boards);
    }
    else if (option == "-n") {
        if (argc > 5 || argc < 3) {
            cout << "Usage: sudoku.exe -n <count> -m <difficulty> or sudoku.exe -n <count> -r <min>~<max> or sudoku.exe -n <count> -u" << endl;
            return 1;
        }
        int count = atoi(argv[2]);//棋盘数量

        if (count <= 0 || count > 10000) {
            cout << "Invalid count: " << argv[2] << endl;
            return 1;
        }

        if (argc == 3) {
            vector<board_t> boards = generate_sudoku_games(count, 0, false, range_t(25, 55));
            for (int i = 0; i < boards.size(); i++) {
                cout<<"Generate the "<<i + 1<<" game:"<<endl;
                print_board(boards[i]);
            }
            write_boards("game.txt", boards);
            return 0;
        }

        string subOption = argv[3];//m or r

        if (subOption == "-m") {
            if (argc != 5) {
                cout << "Usage: sudoku.exe -n <count> -m <difficulty>" << endl;
                return 1;
            }

            int difficulty = atoi(argv[4]);//难度等级

            if (difficulty < 1 || difficulty > 3) {
                cout << "Invalid difficulty level: " << argv[4] << endl;
                return 1;
            }

            
            vector<board_t> boards = generate_sudoku_games(count, difficulty, false, range_t(25, 55));
            for (int i = 0; i < boards.size(); i++) {
                cout<<"Generate the "<<i + 1<<" game:"<<endl;
                print_board(boards[i]);
            }
            write_boards("game.txt", boards);
        }
        else if (subOption == "-r") {
            if (argc != 5) {
                cout << "Usage: sudoku.exe -n <count> -r <min>~<max>" << endl;
                return 1;
            }

            string range = argv[4];
            size_t tildeIndex = range.find("~");

            if (tildeIndex == string::npos || tildeIndex == 0 || tildeIndex == range.length() - 1) {
                cout << "Invalid range format: " << argv[4] << endl;
                return 1;
            }

            int minRange = atoi(range.substr(0, tildeIndex).c_str());
            int maxRange = atoi(range.substr(tildeIndex + 1).c_str());

            if (minRange >= maxRange || maxRange > 55 || minRange < 20) {
                cout << "Invalid range values: " << argv[4] << endl;
                return 1;
            }
            
            vector<board_t> boards = generate_sudoku_games(count, -1, false, range_t(minRange,maxRange));
            for (int i = 0; i < boards.size(); i++) {
                cout<<"Generate the "<<i + 1<<" game:"<<endl;
                print_board(boards[i]);
            }
            write_boards("game.txt", boards);
        }
        else if (subOption == "-u") {
            if (argc != 4) {
                cout << "Usage: sudoku.exe -n <count> -u" << endl;
                return 1;
            }

            
            vector<board_t> boards = generate_sudoku_games(count, 0, true, range_t(25, 55));
            for (int i = 0; i < boards.size(); i++) {
                cout<<"Generate the "<<i + 1<<" game:"<<endl;
                print_board(boards[i]);
            }
            write_boards("game.txt", boards);
        }
        else {
            cout << "Invalid sub-option: " << subOption << endl;
            return 1;
        }
    }
    else {
        cout << "Invalid option: " << option << endl;
        return 1;
    }

    return 0;
}

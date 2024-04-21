#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <queue> 

using std::cout;
using std::endl;

#define DIMENSION 9
#define ZERO 0
#define FULL std::make_pair(9, 9)


struct State_ucs {
    int sudoku[DIMENSION][DIMENSION];
    int row;
    int col;
    int cost; 

    bool operator>(const State_ucs& other) const {
        return cost > other.cost;
    }
};

bool verify_row_ucs(int sudoku[DIMENSION][DIMENSION], int row, int num) {
    for (int col = 0; col < DIMENSION; col++) {
        if (sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool verify_col_ucs(int sudoku[DIMENSION][DIMENSION], int col, int num) {
    for (int row = 0; row < DIMENSION; row++) {
        if (sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool verify_box_ucs(int sudoku[DIMENSION][DIMENSION], int box_start_row, int box_start_col, int num) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (sudoku[row + box_start_row][col + box_start_col] == num) {
                return true;
            }
        }
    }
    return false;
}

bool can_i_use_ucs(int sudoku[DIMENSION][DIMENSION], int row, int col, int num) {
    return !verify_row_ucs(sudoku, row, num) && !verify_col_ucs(sudoku, col, num) && !verify_box_ucs(sudoku, row - row % 3, col - col % 3, num);
}

std::pair<int, int> next_step_ucs(int sudoku[DIMENSION][DIMENSION]) {
    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
            if (sudoku[row][col] == ZERO) {
                return std::make_pair(row, col);
            }
        }
    }
    return FULL;
}

bool goal_ucs(int sudoku[DIMENSION][DIMENSION]) {
    return next_step_ucs(sudoku) == FULL;
}

bool ucs_solution(int sudoku[DIMENSION][DIMENSION]) {

    std::priority_queue<State_ucs, std::vector<State_ucs>, std::greater<State_ucs>> dijkstra_queue;

    State_ucs initial_state;
    std::pair<int, int> initial_pos = next_step_ucs(sudoku);
    initial_state.row = initial_pos.first;
    initial_state.col = initial_pos.second;
    initial_state.cost = 0;
    int expansions = 0;

    std::copy(&sudoku[0][0], &sudoku[0][0] + DIMENSION * DIMENSION, &initial_state.sudoku[0][0]);

    dijkstra_queue.push(initial_state);

    while (!dijkstra_queue.empty()) {

        State_ucs current_state = dijkstra_queue.top();
        dijkstra_queue.pop();
        expansions++;

        if (goal_ucs(current_state.sudoku)) {
            std::copy(&current_state.sudoku[0][0], &current_state.sudoku[0][0] + DIMENSION * DIMENSION, &sudoku[0][0]);
            cout << "Number of expansions : "<< expansions << endl;
            return true;
        }

        for (int num = 1; num <= 9; num++) {
            if (can_i_use_ucs(current_state.sudoku, current_state.row, current_state.col, num)) {
                
                State_ucs next_state = current_state;
                next_state.sudoku[next_state.row][next_state.col] = num;
                std::pair<int, int> next_pos = next_step_ucs(next_state.sudoku);
                next_state.row = next_pos.first;
                next_state.col = next_pos.second;
                next_state.cost++;
                dijkstra_queue.push(next_state);
            }
        }
    }
    
    return false;
}

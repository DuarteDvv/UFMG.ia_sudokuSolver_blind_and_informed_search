#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <stack>

using std::cout;
using std::endl;

#define DIMENSION 9
#define ZERO 0
#define FULL std::make_pair(9, 9)

struct State_ids {
    int sudoku[DIMENSION][DIMENSION];
    int row;
    int col;
    int depth; 
};

bool verify_row_ids(int sudoku[DIMENSION][DIMENSION], int row, int num) {
    for (int col = 0; col < DIMENSION; col++) {
        if (sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool verify_col_ids(int sudoku[DIMENSION][DIMENSION], int col, int num) {
    for (int row = 0; row < DIMENSION; row++) {
        if (sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool verify_box_ids(int sudoku[DIMENSION][DIMENSION], int box_start_row, int box_start_col, int num) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (sudoku[row + box_start_row][col + box_start_col] == num) {
                return true;
            }
        }
    }
    return false;
}

bool can_i_use_ids(int sudoku[DIMENSION][DIMENSION], int row, int col, int num) {
    return !verify_row_ids(sudoku, row, num) && !verify_col_ids(sudoku, col, num) && !verify_box_ids(sudoku, row - row % 3, col - col % 3, num);
}

std::pair<int, int> next_step_ids(int sudoku[DIMENSION][DIMENSION]) {
    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
            if (sudoku[row][col] == ZERO) {
                return std::make_pair(row, col);
            }
        }
    }
    return FULL;
}

bool goal_ids(int sudoku[DIMENSION][DIMENSION]) {
    return next_step_ids(sudoku) == FULL;
}

int depth_limited_search(int sudoku[DIMENSION][DIMENSION], int depth_limit) {
    std::stack<State_ids> dfs_stack;
    int expansions = 0;  
    
    State_ids initial_state;
    std::pair<int, int> initial_pos = next_step_ids(sudoku);
    initial_state.row = initial_pos.first;
    initial_state.col = initial_pos.second;
    std::copy(&sudoku[0][0], &sudoku[0][0] + DIMENSION * DIMENSION, &initial_state.sudoku[0][0]);
    initial_state.depth = 0; 
    dfs_stack.push(initial_state);

    while (!dfs_stack.empty()) {
        State_ids current_state = dfs_stack.top();
        dfs_stack.pop();

        if (goal_ids(current_state.sudoku)) {
                std::copy(&current_state.sudoku[0][0], &current_state.sudoku[0][0] + DIMENSION * DIMENSION, &sudoku[0][0]);
                return expansions;  
            }
          

        if (current_state.depth <= depth_limit) { 

            expansions++;
            
            for (int num = 1; num <= 9; num++) {
                if (can_i_use_ids(current_state.sudoku, current_state.row, current_state.col, num)) {
                    State_ids next_state = current_state;
                    next_state.sudoku[next_state.row][next_state.col] = num;
                    std::pair<int, int> next_pos = next_step_ids(next_state.sudoku);
                    next_state.row = next_pos.first;
                    next_state.col = next_pos.second;
                    next_state.depth++; 
                    dfs_stack.push(next_state);
                }
            }
        }
    }

    return expansions; 
}

bool ids_solution(int sudoku[DIMENSION][DIMENSION]) {
    int total_expansions = 0;
    int depth_limit = 0; 

    while (true) {
        int expansions = depth_limited_search(sudoku, depth_limit);
        if (expansions > 0) {
            total_expansions += expansions;
            depth_limit++; 
        } else {
            break; 
        }
    }

    cout << "Total number of expansions: " << total_expansions << endl;

    return total_expansions > 0;
}

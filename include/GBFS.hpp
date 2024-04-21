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

struct State_gbfs {
    int sudoku[DIMENSION][DIMENSION];
    int row;
    int col;
    int cost; 
};

bool verify_row_gbfs(const State_gbfs& state, int row, int num) {
    for (int col = 0; col < DIMENSION; col++) {
        if (state.sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool verify_col_gbfs(const State_gbfs& state, int col, int num) {
    for (int row = 0; row < DIMENSION; row++) {
        if (state.sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool verify_box_gbfs(const State_gbfs& state, int box_start_row, int box_start_col, int num) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (state.sudoku[row + box_start_row][col + box_start_col] == num) {
                return true;
            }
        }
    }
    return false;
}

bool can_i_use_gbfs(const State_gbfs& state, int row, int col, int num) {
    return !verify_row_gbfs(state, row, num) && !verify_col_gbfs(state, col, num) && !verify_box_gbfs(state, row - row % 3, col - col % 3, num);
}

std::pair<int, int> next_step_gbfs(const State_gbfs& state) {
    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
            if (state.sudoku[row][col] == ZERO) {
                return std::make_pair(row, col);
            }
        }
    }
    return FULL;
}

bool goal_gbfs(const State_gbfs& state) {
    return next_step_gbfs(state) == FULL;
}

struct CompareStates {
    bool operator()(const State_gbfs& s1, const State_gbfs& s2) {
        return s1.cost > s2.cost; 
    }
};


int heuristic(const State_gbfs& state, int row, int col) {
    int constraints = 0;

    // Verificar restrições na linha
    for (int c = 0; c < DIMENSION; ++c) {
        if (c != col && state.sudoku[row][c] == ZERO) {
            constraints += std::count_if(state.sudoku[row], state.sudoku[row] + DIMENSION, [state, row, c](int num) {
                return num == ZERO && !can_i_use_gbfs(state, row, c, num);
            });
        }
    }

    // Verificar restrições na coluna
    for (int r = 0; r < DIMENSION; ++r) {
        if (r != row && state.sudoku[r][col] == ZERO) {
            constraints += std::count_if(state.sudoku[r], state.sudoku[r] + DIMENSION, [state, r, col](int num) {
                return num == ZERO && !can_i_use_gbfs(state, r, col, num);
            });
        }
    }

    // Verificar restrições no bloco
    int box_start_row = row - row % 3;
    int box_start_col = col - col % 3;
    for (int r = box_start_row; r < box_start_row + 3; ++r) {
        for (int c = box_start_col; c < box_start_col + 3; ++c) {
            if ((r != row || c != col) && state.sudoku[r][c] == ZERO) {
                constraints += std::count_if(state.sudoku[r], state.sudoku[r] + DIMENSION, [state, r, c](int num) {
                    return num == ZERO && !can_i_use_gbfs(state, r, c, num);
                });
            }
        }
    }

    return constraints;
}

bool greedy_best_first_search(int sudoku[DIMENSION][DIMENSION]) {
    int expansions = 0;

    State_gbfs initial_state;
    std::copy(&sudoku[0][0], &sudoku[0][0] + DIMENSION * DIMENSION, &initial_state.sudoku[0][0]);
    initial_state.row = -1; 
    initial_state.col = -1; 
    initial_state.cost = 0; 

    std::priority_queue<State_gbfs, std::vector<State_gbfs>, CompareStates> frontier;
    frontier.push(initial_state);

    while (!frontier.empty()) {
        State_gbfs current_state = frontier.top();
        frontier.pop();
        expansions++;

        if (goal_gbfs(current_state)) {
            std::copy(&current_state.sudoku[0][0], &current_state.sudoku[0][0] + DIMENSION * DIMENSION, &sudoku[0][0]);
            cout << "Number of expansions : "<< expansions << endl;
            return true;
        }

        std::pair<int, int> empty_cell = next_step_gbfs(current_state);
        int row = empty_cell.first;
        int col = empty_cell.second;

        std::vector<std::pair<int, int>> possible_values;
        for (int num = 1; num <= 9; num++) {
            if (can_i_use_gbfs(current_state, row, col, num)) {
                int cost = heuristic(current_state, row, col);
                possible_values.push_back(std::make_pair(cost, num));
            }
        }

        std::sort(possible_values.begin(), possible_values.end());

        for (const auto& value_pair : possible_values) {
            int num = value_pair.second;
            State_gbfs next_state = current_state;
            next_state.sudoku[row][col] = num;
            next_state.cost = value_pair.first; 
            frontier.push(next_state);
        }
    }

    return false;
}
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <queue>
#include <cmath> 

using std::cout;
using std::endl;

#define DIMENSION 9
#define ZERO 0
#define FULL std::make_pair(9, 9)

struct State_a {
    int sudoku[DIMENSION][DIMENSION];
    int row;
    int col;
    int g_cost; // Custo do caminho percorrido até o estado atual
    int h_cost; // Heurística: custo estimado do estado atual até o objetivo
};

// Função para verificar se o número está sendo usado na linha
bool verify_row_a(int sudoku[DIMENSION][DIMENSION], int row, int num) {
    for (int col = 0; col < DIMENSION; col++) {
        if (sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

// Função para verificar se o número está sendo usado na coluna
bool verify_col_a(int sudoku[DIMENSION][DIMENSION], int col, int num) {
    for (int row = 0; row < DIMENSION; row++) {
        if (sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

// Função para verificar se o número está sendo usado na caixa 3x3
bool verify_box_a(int sudoku[DIMENSION][DIMENSION], int box_start_row, int box_start_col, int num) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (sudoku[row + box_start_row][col + box_start_col] == num) {
                return true;
            }
        }
    }
    return false;
}

// Função para verificar se é seguro atribuir um número a uma determinada posição
bool can_i_use_a(int sudoku[DIMENSION][DIMENSION], int row, int col, int num) {
    return !verify_row_a(sudoku, row, num) && !verify_col_a(sudoku, col, num) && !verify_box_a(sudoku, row - row % 3, col - col % 3, num);
}


std::pair<int, int> next_step_a(int sudoku[DIMENSION][DIMENSION]) {
    int min_options = 10; 
    std::pair<int, int> min_pos;

    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
            if (sudoku[row][col] == ZERO) {
                int num_options = 0;
                for (int num = 1; num <= 9; num++) {
                    if (can_i_use_a(sudoku, row, col, num)) {
                        num_options++;
                    }
                }
                if (num_options < min_options) {
                    min_options = num_options;
                    min_pos = std::make_pair(row, col);
                }
            }
        }
    }
    if (min_options == 10) {
        return FULL; 
    }
    return min_pos;
}


bool goal_a(int sudoku[DIMENSION][DIMENSION]) {
    return next_step_a(sudoku) == FULL;
}

// Função de heurística: número de opções disponíveis para cada célula vazia
int heuristic(int sudoku[DIMENSION][DIMENSION]) {
    int total_options = 0;
    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
            if (sudoku[row][col] == ZERO) {
                for (int num = 1; num <= 9; num++) {
                    if (can_i_use_a(sudoku, row, col, num)) {
                        total_options++;
                    }
                }
            }
        }
    }
    return total_options;
}


struct CompareState {
    bool operator()(const State_a& s1, const State_a& s2) const {
        // A* = g(n) + h(n), onde g(n) é o custo do caminho percorrido até o estado atual e h(n) é a heurística (número de opções disponíveis)
        return s1.g_cost + s1.h_cost > s2.g_cost + s2.h_cost;
    }
};


bool a_star_solution(int sudoku[DIMENSION][DIMENSION]) {
    std::priority_queue<State_a, std::vector<State_a>, CompareState> frontier; 
    int expansions = 0;

    
    State_a initial_state;
    std::pair<int, int> initial_pos = next_step_a(sudoku);
    initial_state.row = initial_pos.first;
    initial_state.col = initial_pos.second;
    initial_state.g_cost = 0;
    initial_state.h_cost = heuristic(sudoku);

    std::copy(&sudoku[0][0], &sudoku[0][0] + DIMENSION * DIMENSION, &initial_state.sudoku[0][0]);

    frontier.push(initial_state);

    while (!frontier.empty()) {
        State_a current_state = frontier.top();
        frontier.pop();
        expansions++;

        if (goal_a(current_state.sudoku)) {
            std::copy(&current_state.sudoku[0][0], &current_state.sudoku[0][0] + DIMENSION * DIMENSION, &sudoku[0][0]);
            cout << "Number of expansions: " << expansions << endl;
            return true;
        }

        for (int num = 1; num <= 9; num++) {
            if (can_i_use_a(current_state.sudoku, current_state.row, current_state.col, num)) {
                State_a next_state = current_state;
                next_state.sudoku[next_state.row][next_state.col] = num;
                std::pair<int, int> next_pos = next_step_a(next_state.sudoku);
                next_state.row = next_pos.first;
                next_state.col = next_pos.second;
                next_state.g_cost++; 
                next_state.h_cost = heuristic(next_state.sudoku); 
                frontier.push(next_state);
            }
        }
    }
    return false;
}
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


struct State_bfs {
    int sudoku[DIMENSION][DIMENSION];
    int row;
    int col;
};

// Ja foi usado na linha ?
bool verify_row_bfs(int sudoku[DIMENSION][DIMENSION], int row, int num) {
    for (int col = 0; col < DIMENSION; col++) {
        if (sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

// Ja foi usado na coluna ?
bool verify_col_bfs(int sudoku[DIMENSION][DIMENSION], int col, int num) {
    for (int row = 0; row < DIMENSION; row++) {
        if (sudoku[row][col] == num) {
            return true;
        }
    }
    return false;
}

// Ja foi usado no sudoku 3x3?
bool verify_box_bfs(int sudoku[DIMENSION][DIMENSION], int box_start_row, int box_start_col, int num) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (sudoku[row + box_start_row][col + box_start_col] == num) {
                return true;
            }
        }
    }
    return false;
}

// Verifica se o numero é valido
bool can_i_use_bfs(int sudoku[DIMENSION][DIMENSION], int row, int col, int num) {
    return !verify_row_bfs(sudoku, row, num) && !verify_col_bfs(sudoku, col, num) && !verify_box_bfs(sudoku, row - row % 3, col - col % 3, num);
}

// Encontra a próxima posição com 0
std::pair<int, int> next_step_bfs(int sudoku[DIMENSION][DIMENSION]) {
    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
            if (sudoku[row][col] == ZERO) {
                return std::make_pair(row, col);
            }
        }
    }
    return FULL;
}

bool goal_bfs(int sudoku[DIMENSION][DIMENSION]) {
    return next_step_bfs(sudoku) == FULL;
}

bool bfs_solution(int sudoku[DIMENSION][DIMENSION]) {

    std::queue<State_bfs> bfs_queue;

    int expansions = 0;

    // Adiciona o estado inicial do tabuleiro à fila
    State_bfs initial_state;
    std::pair<int, int> initial_pos = next_step_bfs(sudoku);
    initial_state.row = initial_pos.first;
    initial_state.col = initial_pos.second;

    std::copy(&sudoku[0][0], &sudoku[0][0] + DIMENSION * DIMENSION, &initial_state.sudoku[0][0]);

    bfs_queue.push(initial_state);

    while (!bfs_queue.empty()) {

        State_bfs current_state = bfs_queue.front();
        bfs_queue.pop();
        expansions++;

        if (goal_bfs(current_state.sudoku)) { // se ja estiver preenchido retorna true
            std::copy(&current_state.sudoku[0][0], &current_state.sudoku[0][0] + DIMENSION * DIMENSION, &sudoku[0][0]);
            cout << "Number of expansions : "<< expansions << endl;
            return true;
        }

        // Para cada número de 1 a 9, tenta atribuir à próxima posição vazia
        for (int num = 1; num <= 9; num++) {
            if (can_i_use_bfs(current_state.sudoku, current_state.row, current_state.col, num)) { //Verifica se o numero é valido 
                
                State_bfs next_state = current_state;
                next_state.sudoku[next_state.row][next_state.col] = num;
                std::pair<int, int> next_pos = next_step_bfs(next_state.sudoku);
                next_state.row = next_pos.first;
                next_state.col = next_pos.second;
                bfs_queue.push(next_state); //Adiciona os filhos a fila
            }
        }
    }

    
    return false;
}
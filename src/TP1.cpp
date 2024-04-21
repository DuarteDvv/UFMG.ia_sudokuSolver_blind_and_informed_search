#include <BFS.hpp>
#include <UCS.hpp>
#include <A_star.hpp>
#include <IDS.hpp>
#include <GBFS.hpp>
#include <iostream>
#include <iomanip>
#include <chrono>

#define SPACE " "
#define LINE "|"
#define NEW_ROW "-------------------------------------"


void print_grid(int grid[DIMENSION][DIMENSION])
{
	for (int i = 0; i < DIMENSION; i++)
	{
		cout << SPACE << SPACE << SPACE << SPACE << endl;
		cout << NEW_ROW << endl;
		for (int j = 0; j < DIMENSION; j++)
		{
			cout << SPACE;
			if (ZERO == grid[i][j])
			{
				cout << SPACE;
			}
			else
			{
				cout << grid[i][j];
			}
			cout << SPACE;
			cout << LINE;
		}
	}
	cout << endl << NEW_ROW << endl << endl;;
}

int main(){

    int sudoku[9][9];;
    char alg;

    std::cin >> alg;

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            std::cin >> sudoku[i][j];
        }
    }

    auto start_time = std::chrono::steady_clock::now();

    // SOLUTIONS
    switch (alg)
    {
        case 'B':  //Breadth-first search
            if(bfs_solution(sudoku)){
                break;
            }
            else{
                cout << "NO SOLUTION"<< endl; 
                break;
            }
        case 'I': //Iterative deepening search
            if(ids_solution(sudoku)){
                break; 
            }
            else {
                cout << "NO SOLUTION"<< endl; 
                break;
            }
        case 'U':  //Uniform-cost search
            if(ucs_solution(sudoku)){
                break; 
            }
            else {
                cout << "NO SOLUTION"<< endl; 
                break;
            }
        case 'A':  // A*
            if(a_star_solution(sudoku)){
                break; 
            }
            else {
                cout << "NO SOLUTION"<< endl; 
                break;
            }
        case 'G':  //Greedy Best-First Search
            if(greedy_best_first_search(sudoku)){
                break; 
            }
            else {
                cout << "NO SOLUTION"<< endl; 
                break;
            }
            
    }

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    auto elapsed_time_ms = elapsed_time_ns / 1000000.0;
    std::cout << std::fixed << std::setprecision(3);

    std::cout << "Execution time: " << elapsed_time_ms << " miliseconds" << std::endl;
    
    print_grid(sudoku);

};

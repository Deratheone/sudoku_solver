#ifndef SUDOKU_H
#define SUDOKU_H

#define SIZE 9

// Function declarations for sudoku solving logic
int is_valid(int grid[SIZE][SIZE], int row, int col, int num);
int solve_sudoku(int grid[SIZE][SIZE]);
void print_grid(int grid[SIZE][SIZE]);
int is_grid_complete(int grid[SIZE][SIZE]);
int is_grid_valid(int grid[SIZE][SIZE]);

// Puzzle generation functions
void shuffle_array(int arr[], int n);
int fill_diagonal_boxes(int grid[SIZE][SIZE]);
int generate_complete_sudoku(int grid[SIZE][SIZE]);
void create_puzzle_from_solution(int solution[SIZE][SIZE], int puzzle[SIZE][SIZE], int difficulty);

#endif

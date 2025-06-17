#ifndef SUDOKU_H
#define SUDOKU_H

#define SIZE 9

// Function declarations for sudoku solving logic
int is_valid(int grid[SIZE][SIZE], int row, int col, int num);
int solve_sudoku(int grid[SIZE][SIZE]);
void print_grid(int grid[SIZE][SIZE]);
int is_grid_complete(int grid[SIZE][SIZE]);

#endif

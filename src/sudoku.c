#include <stdio.h>
#include <string.h>
#include "sudoku.h"

// Check if placing num at grid[row][col] is valid
int is_valid(int grid[SIZE][SIZE], int row, int col, int num) {
    // Check row - no duplicate numbers in the same row
    for (int x = 0; x < SIZE; x++) {
        if (grid[row][x] == num) {
            return 0; // Number already exists in this row
        }
    }
    
    // Check column - no duplicate numbers in the same column
    for (int x = 0; x < SIZE; x++) {
        if (grid[x][col] == num) {
            return 0; // Number already exists in this column
        }
    }
    
    // Check 3x3 box - no duplicate numbers in the same 3x3 section
    int start_row = row - row % 3;  // Find top-left corner of the 3x3 box
    int start_col = col - col % 3;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + start_row][j + start_col] == num) {
                return 0; // Number already exists in this 3x3 box
            }
        }
    }
    
    return 1; // Valid placement - all sudoku rules satisfied
}

// Solve sudoku using backtracking algorithm
int solve_sudoku(int grid[SIZE][SIZE]) {
    int row, col;
    
    // Find the first empty cell (represented by 0)
    int found_empty = 0;
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                found_empty = 1;
                break; // Found empty cell, exit inner loop
            }
        }
        if (found_empty) break; // Exit outer loop if empty cell found
    }
    
    // If no empty cell found, puzzle is completely solved
    if (!found_empty) {
        return 1; // Success! All cells are filled
    }
    
    // Try numbers 1 through 9 in the empty cell
    for (int num = 1; num <= 9; num++) {
        // Check if placing this number is valid
        if (is_valid(grid, row, col, num)) {
            // Place the number temporarily
            grid[row][col] = num;
            
            // Recursively try to solve the rest of the puzzle
            if (solve_sudoku(grid)) {
                return 1; // Solution found!
            }
            
            // If placing this number doesn't lead to a solution,
            // backtrack by removing it and trying the next number
            grid[row][col] = 0;
        }
    }
    
    // No number 1-9 worked in this position, so backtrack
    return 0;
}

// Print the grid to console (useful for debugging)
void print_grid(int grid[SIZE][SIZE]) {
    printf("\nSudoku Grid:\n");
    printf("-------------------------\n");
    
    for (int row = 0; row < SIZE; row++) {
        // Add horizontal separator for 3x3 sections
        if (row % 3 == 0 && row != 0) {
            printf("-------------------------\n");
        }
        
        printf("| ");
        for (int col = 0; col < SIZE; col++) {
            // Add vertical separator for 3x3 sections
            if (col % 3 == 0 && col != 0) {
                printf("| ");
            }
            
            // Print number or space for empty cells
            if (grid[row][col] == 0) {
                printf("  ");
            } else {
                printf("%d ", grid[row][col]);
            }
        }
        printf("|\n");
    }
    printf("-------------------------\n");
}

// Check if the grid is completely filled (no zeros)
int is_grid_complete(int grid[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) {
                return 0; // Found empty cell
            }
        }
    }
    return 1; // All cells are filled
}

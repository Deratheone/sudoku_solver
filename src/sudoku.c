#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
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

// Helper: check if the initial grid is valid (no duplicate numbers in any row, column, or 3x3 box)
int is_grid_valid(int grid[SIZE][SIZE]) {
    // Check rows and columns
    for (int i = 0; i < SIZE; i++) {
        int row_seen[SIZE+1] = {0};
        int col_seen[SIZE+1] = {0};
        for (int j = 0; j < SIZE; j++) {
            int row_val = grid[i][j];
            int col_val = grid[j][i];
            if (row_val) {
                if (row_seen[row_val]) return 0;
                row_seen[row_val] = 1;
            }
            if (col_val) {
                if (col_seen[col_val]) return 0;
                col_seen[col_val] = 1;
            }
        }
    }
    // Check 3x3 boxes
    for (int boxRow = 0; boxRow < 3; boxRow++) {
        for (int boxCol = 0; boxCol < 3; boxCol++) {
            int seen[SIZE+1] = {0};
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int val = grid[boxRow*3 + i][boxCol*3 + j];
                    if (val) {
                        if (seen[val]) return 0;
                        seen[val] = 1;
                    }
                }
            }
        }
    }
    return 1;
}

// Helper function to shuffle array
void shuffle_array(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Fill diagonal 3x3 boxes first (they don't affect each other)
int fill_diagonal_boxes(int grid[SIZE][SIZE]) {
    for (int box = 0; box < SIZE; box += 3) {
        int nums[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        shuffle_array(nums, SIZE);
        
        int index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grid[box + i][box + j] = nums[index++];
            }
        }
    }
    return 1;
}

// Generate a complete valid sudoku solution
int generate_complete_sudoku(int grid[SIZE][SIZE]) {
    // Clear the grid first
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = 0;
        }
    }
    
    // Fill diagonal boxes first
    fill_diagonal_boxes(grid);
    
    // Now solve the rest using backtracking
    return solve_sudoku(grid);
}

// Create a puzzle by removing numbers from a complete solution
void create_puzzle_from_solution(int solution[SIZE][SIZE], int puzzle[SIZE][SIZE], int difficulty) {
    // Copy solution to puzzle
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            puzzle[i][j] = solution[i][j];
        }
    }
    
    // Remove numbers based on difficulty
    // Easy: remove 35-40 numbers, Medium: 45-50, Hard: 55-60
    int numbers_to_remove;
    switch (difficulty) {
        case 0: numbers_to_remove = 35 + rand() % 6; break; // Easy
        case 1: numbers_to_remove = 45 + rand() % 6; break; // Medium  
        case 2: numbers_to_remove = 55 + rand() % 6; break; // Hard
        default: numbers_to_remove = 40; break;
    }
    
    // Create array of all positions
    int positions[SIZE * SIZE][2];
    int pos_count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            positions[pos_count][0] = i;
            positions[pos_count][1] = j;
            pos_count++;
        }
    }
    
    // Shuffle positions
    for (int i = pos_count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp_row = positions[i][0];
        int temp_col = positions[i][1];
        positions[i][0] = positions[j][0];
        positions[i][1] = positions[j][1];
        positions[j][0] = temp_row;
        positions[j][1] = temp_col;
    }
    
    // Remove numbers from random positions
    for (int i = 0; i < numbers_to_remove && i < pos_count; i++) {
        int row = positions[i][0];
        int col = positions[i][1];
        puzzle[row][col] = 0;
    }
}

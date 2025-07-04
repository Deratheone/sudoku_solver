#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include "sudoku.h"
#include <commctrl.h>

// Window dimensions and layout constants
#define WINDOW_WIDTH 650
#define WINDOW_HEIGHT 750
#define CELL_SIZE 55
#define CELL_SPACING 2
#define BOX_SPACING 8
#define GRID_START_X 50
#define GRID_START_Y 80

// Control IDs for buttons
#define ID_SOLVE_BUTTON 1001
#define ID_CLEAR_BUTTON 1002
#define ID_EXAMPLE_BUTTON 1004

// Base ID for sudoku cells (each cell gets unique ID: ID_CELL_BASE + row*9 + col)
#define ID_CELL_BASE 2000

#define TAB_SOLVER 0
#define TAB_GAME 1
#define MAX_HINTS 3

// Tab and control IDs
#define ID_TAB 5000
#define ID_GRID 5001
#define ID_BUTTONS 5002
#define ID_HINTS 5003
#define ID_CHECK 5004
#define ID_GENERATE 5005
#define ID_GAME_TAB 5006

// Function declarations for GUI management
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateSudokuGrid(HWND hwnd);
void CreateButtons(HWND hwnd);
void GetGridFromGUI(HWND hwnd, int grid[SIZE][SIZE]);
void SetGridToGUI(HWND hwnd, int grid[SIZE][SIZE]);
void ClearGrid(HWND hwnd);
void LoadExamplePuzzle(HWND hwnd);
void CreateTabControl(HWND hwnd);
void ShowSolverTab(HWND hwnd, BOOL show);
void ShowGameTab(HWND hwnd, BOOL show);
void CreateGameTab(HWND hwnd);
void GenerateSudokuPuzzle(int puzzle[SIZE][SIZE], int solution[SIZE][SIZE]);
void GiveHint(HWND hwnd);
void CheckUserSolution(HWND hwnd);

#endif

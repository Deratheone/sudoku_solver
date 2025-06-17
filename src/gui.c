#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include "sudoku.h"

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

// Window procedure - handles all messages sent to our window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Window is being created - set up our controls
            CreateSudokuGrid(hwnd);
            CreateButtons(hwnd);
            break;
            
        case WM_COMMAND:
            // A button was clicked or control was activated
            switch (LOWORD(wParam)) {
                case ID_SOLVE_BUTTON: {
                    // Get current grid from GUI
                    int grid[SIZE][SIZE];
                    GetGridFromGUI(hwnd, grid);
                    
                    // Check if the grid is completely empty
                    int isEmpty = 1;
                    for (int i = 0; i < SIZE; i++) {
                        for (int j = 0; j < SIZE; j++) {
                            if (grid[i][j] != 0) {
                                isEmpty = 0;
                                break;
                            }
                        }
                        if (!isEmpty) break;
                    }
                    
                    if (isEmpty) {
                        MessageBox(hwnd, "Please enter at least one number in the grid before solving!", "Grid is empty", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    if (!is_grid_valid(grid)) {
                        MessageBox(hwnd, "The grid has duplicate numbers in a row, column, or box. Please fix your input.", "Invalid Sudoku Input", MB_OK | MB_ICONERROR);
                        break;
                    }
                    // Try to solve the sudoku
                    if (solve_sudoku(grid)) {
                        // Solution found - display it in the GUI
                        SetGridToGUI(hwnd, grid);
                        MessageBox(hwnd, "Sudoku solved successfully!", "Success", 
                                  MB_OK | MB_ICONINFORMATION);
                    } else {
                        // No solution exists
                        MessageBox(hwnd, "No solution exists for this puzzle!\nPlease check your input.", 
                                  "Error", MB_OK | MB_ICONERROR);
                    }
                    break;
                }
                case ID_CLEAR_BUTTON:
                    // Clear all cells in the grid
                    ClearGrid(hwnd);
                    break;
                      case ID_EXAMPLE_BUTTON:
                    // Load an example Sudoku puzzle
                    LoadExamplePuzzle(hwnd);
                    break;
            }
            break;
            
        case WM_CLOSE:
            // User clicked the X button or pressed Alt+F4
            PostQuitMessage(0);
            break;
            
        default:
            // Let Windows handle any other messages
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Create the 9x9 grid of input fields for the sudoku puzzle
void CreateSudokuGrid(HWND hwnd) {
    // Create font for cells
    HFONT cellFont = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
                               DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
                               CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
    
    // First, create 9 background panels for the 3x3 boxes to give visual separation
    for (int boxRow = 0; boxRow < 3; boxRow++) {
        for (int boxCol = 0; boxCol < 3; boxCol++) {
            int boxX = GRID_START_X + boxCol * (3 * CELL_SIZE + 2 * CELL_SPACING + BOX_SPACING);
            int boxY = GRID_START_Y + boxRow * (3 * CELL_SIZE + 2 * CELL_SPACING + BOX_SPACING);
            int boxWidth = 3 * CELL_SIZE + 2 * CELL_SPACING;
            int boxHeight = 3 * CELL_SIZE + 2 * CELL_SPACING;
            
            // Create a panel for each 3x3 box with a dark border
            CreateWindowEx(
                WS_EX_STATICEDGE,  // Sunken border
                "STATIC", "",
                WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,
                boxX - 3, boxY - 3, boxWidth + 6, boxHeight + 6,
                hwnd, (HMENU)(3000 + boxRow * 3 + boxCol),
                GetModuleHandle(NULL), NULL
            );
        }
    }
    
    // Now create individual cells
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            // Calculate which 3x3 box this cell belongs to
            int boxRow = row / 3;
            int boxCol = col / 3;
            
            // Calculate position within the box
            int cellInBoxRow = row % 3;
            int cellInBoxCol = col % 3;
            
            // Calculate absolute position
            int x = GRID_START_X + boxCol * (3 * CELL_SIZE + 2 * CELL_SPACING + BOX_SPACING)
                   + cellInBoxCol * (CELL_SIZE + CELL_SPACING);
            int y = GRID_START_Y + boxRow * (3 * CELL_SIZE + 2 * CELL_SPACING + BOX_SPACING)
                   + cellInBoxRow * (CELL_SIZE + CELL_SPACING);
              // Create the cell with a clean white background
            HWND cell = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                "EDIT", "",
                WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                x, y, CELL_SIZE, CELL_SIZE,
                hwnd,
                (HMENU)(ID_CELL_BASE + row * SIZE + col),
                GetModuleHandle(NULL), NULL
            );
            
            // Set font and properties
            SendMessage(cell, WM_SETFONT, (WPARAM)cellFont, MAKELPARAM(TRUE, 0));
            SendMessage(cell, EM_SETLIMITTEXT, 1, 0);  // Limit to 1 character
            
            // Set white background for the cell
            SetWindowLongPtr(cell, GWL_EXSTYLE, 
                            GetWindowLongPtr(cell, GWL_EXSTYLE) | WS_EX_STATICEDGE);
        }
    }
}

// Create the control buttons (Solve, Clear, Example)
void CreateButtons(HWND hwnd) {
    // Create a nice font for buttons
    HFONT buttonFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
                                DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
                                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
                               // Calculate grid size and button positions
    int gridWidth = 3 * (3 * CELL_SIZE + 2 * CELL_SPACING + BOX_SPACING);
    int gridHeight = gridWidth;  // Grid is square
      // Position buttons with enough space below the grid
    int buttonY = GRID_START_Y + gridHeight + 40;
    int buttonWidth = 130;
    int buttonHeight = 40;
    int buttonSpacing = 20;
    int totalButtonWidth = 3 * buttonWidth + 2 * buttonSpacing;
    int startX = (WINDOW_WIDTH - totalButtonWidth) / 2;
    
    // Create a panel behind the buttons to make them stand out
    HWND buttonPanel = CreateWindowEx(
        WS_EX_STATICEDGE,
        "STATIC", "",
        WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME,
        startX - 20, buttonY - 10, 
        totalButtonWidth + 40, buttonHeight + 20,
        hwnd, (HMENU)2998,
        GetModuleHandle(NULL), NULL
    );
    (void)buttonPanel; // Suppress unused variable warning
      // Solve button
    HWND solveBtn = CreateWindowEx(
        WS_EX_STATICEDGE,
        "BUTTON", "Solve Puzzle",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT,
        startX, buttonY, buttonWidth, buttonHeight,
        hwnd, (HMENU)ID_SOLVE_BUTTON,
        GetModuleHandle(NULL), NULL);
    SendMessage(solveBtn, WM_SETFONT, (WPARAM)buttonFont, MAKELPARAM(TRUE, 0));
        
    // Clear button
    HWND clearBtn = CreateWindowEx(
        WS_EX_STATICEDGE,
        "BUTTON", "Clear Grid",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT,
        startX + buttonWidth + buttonSpacing, buttonY, buttonWidth, buttonHeight,
        hwnd, (HMENU)ID_CLEAR_BUTTON,
        GetModuleHandle(NULL), NULL);
    SendMessage(clearBtn, WM_SETFONT, (WPARAM)buttonFont, MAKELPARAM(TRUE, 0));
    
    // Example button
    HWND exampleBtn = CreateWindowEx(
        WS_EX_STATICEDGE,
        "BUTTON", "Load Example",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT,
        startX + 2 * (buttonWidth + buttonSpacing), buttonY, buttonWidth, buttonHeight,
        hwnd, (HMENU)ID_EXAMPLE_BUTTON,
        GetModuleHandle(NULL), NULL);
    SendMessage(exampleBtn, WM_SETFONT, (WPARAM)buttonFont, MAKELPARAM(TRUE, 0));
      // Title above the grid
    HWND titleLabel = CreateWindow("STATIC", "Sudoku Solver",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        0, 20, WINDOW_WIDTH, 35,
        hwnd, (HMENU)2999,
        GetModuleHandle(NULL), NULL);
        
    // Title font
    HFONT titleFont = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
                                DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
                                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    SendMessage(titleLabel, WM_SETFONT, (WPARAM)titleFont, MAKELPARAM(TRUE, 0));
}

// Read the current values from all GUI cells into a 2D array
void GetGridFromGUI(HWND hwnd, int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            // Get handle to this specific cell
            HWND cellHwnd = GetDlgItem(hwnd, ID_CELL_BASE + row * SIZE + col);
            char buffer[10];
            
            // Get the text from this cell
            GetWindowText(cellHwnd, buffer, sizeof(buffer));
            
            if (strlen(buffer) > 0) {
                // Convert text to number
                int value = atoi(buffer);
                // Validate: only accept numbers 1-9
                if (value >= 1 && value <= 9) {
                    grid[row][col] = value;
                } else {
                    grid[row][col] = 0; // Invalid input treated as empty
                }
            } else {
                grid[row][col] = 0; // Empty cell
            }
        }
    }
}

// Set values from a 2D array into all GUI cells
void SetGridToGUI(HWND hwnd, int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            // Get handle to this specific cell
            HWND cellHwnd = GetDlgItem(hwnd, ID_CELL_BASE + row * SIZE + col);
            char buffer[10];
            
            if (grid[row][col] != 0) {
                // Convert number to text
                sprintf(buffer, "%d", grid[row][col]);
            } else {
                // Empty cell
                strcpy(buffer, "");
            }
            
            // Set the text in this cell
            SetWindowText(cellHwnd, buffer);
        }
    }
}

// Clear all cells in the grid
void ClearGrid(HWND hwnd) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            // Get handle to this specific cell
            HWND cellHwnd = GetDlgItem(hwnd, ID_CELL_BASE + row * SIZE + col);
            // Set empty text
            SetWindowText(cellHwnd, "");
        }
    }
}

// Load an example Sudoku puzzle
void LoadExamplePuzzle(HWND hwnd) {
    // Medium difficulty puzzle
    int example[SIZE][SIZE] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    
    // Load the example into the grid
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            HWND cellHwnd = GetDlgItem(hwnd, ID_CELL_BASE + row * SIZE + col);
            char buffer[10];
            
            if (example[row][col] != 0) {
                sprintf(buffer, "%d", example[row][col]);
            } else {
                strcpy(buffer, "");
            }
            
            SetWindowText(cellHwnd, buffer);
        }
    }
    
    // Notify user
    MessageBox(hwnd, "Example puzzle loaded!\nClick 'Solve Puzzle' to see the solution.", 
              "Example Loaded", MB_OK | MB_ICONINFORMATION);
}

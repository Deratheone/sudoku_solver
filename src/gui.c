#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <time.h>
#include "gui.h"
#include "sudoku.h"
#pragma comment(lib, "comctl32.lib")

// Global for tracking hints left in game mode
static int hints_left = MAX_HINTS;
// Global for storing current puzzle solution in game mode
static int current_solution[SIZE][SIZE];
static int has_puzzle = 0;

// Window procedure - handles all messages sent to our window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hTab = NULL;
    switch (uMsg) {        case WM_CREATE: {
            // Initialize common controls
            InitCommonControls();
            
            // Create tab control
            hTab = CreateWindowEx(0, WC_TABCONTROLA, "", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
                0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hwnd, (HMENU)ID_TAB, GetModuleHandle(NULL), NULL);
            
            // Add tabs
            TCITEMA tie;
            tie.mask = TCIF_TEXT;
            tie.pszText = "Solver";
            TabCtrl_InsertItem(hTab, TAB_SOLVER, &tie);
            tie.pszText = "Game";
            TabCtrl_InsertItem(hTab, TAB_GAME, &tie);
            
            // Create the UI elements
            CreateSudokuGrid(hwnd);
            CreateButtons(hwnd);
            CreateGameTab(hwnd);
            
            ShowSolverTab(hwnd, TRUE);
            ShowGameTab(hwnd, FALSE);
            break;
        }
        case WM_NOTIFY: {
            LPNMHDR pnmh = (LPNMHDR)lParam;
            if (pnmh->code == TCN_SELCHANGE && pnmh->hwndFrom == GetDlgItem(hwnd, ID_TAB)) {
                int sel = TabCtrl_GetCurSel(pnmh->hwndFrom);
                ShowSolverTab(hwnd, sel == TAB_SOLVER);
                ShowGameTab(hwnd, sel == TAB_GAME);
            }
            break;
        }
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
                }                case ID_CLEAR_BUTTON:
                    // Clear all cells in the grid
                    ClearGrid(hwnd);
                    break;
                      case ID_EXAMPLE_BUTTON:
                    // Load an example Sudoku puzzle
                    LoadExamplePuzzle(hwnd);
                    break;                case ID_GENERATE: {
                    // Generate a new puzzle
                    static int puzzle[SIZE][SIZE];
                    GenerateSudokuPuzzle(puzzle, current_solution);
                    SetGridToGUI(hwnd, puzzle);
                    hints_left = MAX_HINTS; // Reset hints
                    has_puzzle = 1; // Mark that we have a puzzle with known solution
                    MessageBox(hwnd, "New puzzle generated! Try to solve it.", "New Puzzle", MB_OK | MB_ICONINFORMATION);
                    break;
                }                case ID_CHECK: {
                    // Check if current solution is correct
                    int grid[SIZE][SIZE];
                    GetGridFromGUI(hwnd, grid);
                    
                    // Check if grid is complete
                    if (!is_grid_complete(grid)) {
                        MessageBox(hwnd, "Please fill in all cells before checking the solution.", "Incomplete", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    // If we have a generated puzzle, compare with known solution
                    if (has_puzzle) {
                        int matches = 1;
                        for (int i = 0; i < SIZE && matches; i++) {
                            for (int j = 0; j < SIZE && matches; j++) {
                                if (grid[i][j] != current_solution[i][j]) {
                                    matches = 0;
                                }
                            }
                        }
                        
                        if (matches) {
                            MessageBox(hwnd, "Congratulations! Your solution is perfectly correct!", "Perfect Solution", MB_OK | MB_ICONEXCLAMATION);
                        } else {
                            MessageBox(hwnd, "Your solution doesn't match the intended answer. Keep trying!", "Try Again", MB_OK | MB_ICONERROR);
                        }
                    } else {
                        // Check if solution is valid (fallback for user-entered puzzles)
                        if (is_grid_valid(grid)) {
                            MessageBox(hwnd, "Your solution follows all Sudoku rules!", "Valid Solution", MB_OK | MB_ICONEXCLAMATION);
                        } else {
                            MessageBox(hwnd, "Your solution has errors. Keep trying!", "Invalid Solution", MB_OK | MB_ICONERROR);
                        }
                    }
                    break;
                }case ID_HINTS: {
                    // Provide a hint
                    if (hints_left <= 0) {
                        MessageBox(hwnd, "No more hints available for this puzzle.", "No Hints", MB_OK | MB_ICONINFORMATION);
                        break;
                    }
                    
                    if (!has_puzzle) {
                        MessageBox(hwnd, "Please generate a puzzle first before asking for hints.", "No Puzzle", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    // Get current grid and find an empty cell to fill
                    int grid[SIZE][SIZE];
                    GetGridFromGUI(hwnd, grid);
                    
                    // Find first empty cell and fill it with the correct solution
                    int found = 0;
                    for (int i = 0; i < SIZE && !found; i++) {
                        for (int j = 0; j < SIZE && !found; j++) {
                            if (grid[i][j] == 0) {
                                grid[i][j] = current_solution[i][j];
                                found = 1;
                            }
                        }
                    }
                    
                    if (found) {
                        SetGridToGUI(hwnd, grid);
                        hints_left--;
                        char message[100];
                        sprintf(message, "Hint provided! %d hints remaining.", hints_left);
                        MessageBox(hwnd, message, "Hint", MB_OK | MB_ICONINFORMATION);
                    } else {
                        MessageBox(hwnd, "The puzzle appears to be complete!", "No Hints Needed", MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                }
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

void ShowSolverTab(HWND hwnd, BOOL show) {
    // Show/hide solver tab controls
    ShowWindow(GetDlgItem(hwnd, ID_SOLVE_BUTTON), show ? SW_SHOW : SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_CLEAR_BUTTON), show ? SW_SHOW : SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_EXAMPLE_BUTTON), show ? SW_SHOW : SW_HIDE);
}

void ShowGameTab(HWND hwnd, BOOL show) {
    // Show/hide game tab controls
    ShowWindow(GetDlgItem(hwnd, ID_GENERATE), show ? SW_SHOW : SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_CHECK), show ? SW_SHOW : SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_HINTS), show ? SW_SHOW : SW_HIDE);
}

void CreateGameTab(HWND hwnd) {
    // Create a nice font for buttons
    HFONT buttonFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
                                DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
                                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    
    // Use the same positioning logic as in CreateButtons
    int gridWidth = 3 * (3 * CELL_SIZE + 2 * CELL_SPACING + BOX_SPACING);
    int gridHeight = gridWidth;
    int buttonY = GRID_START_Y + gridHeight + 40;
    int buttonWidth = 130;
    int buttonHeight = 40;
    int buttonSpacing = 20;
    int totalButtonWidth = 3 * buttonWidth + 2 * buttonSpacing;
    int startX = (WINDOW_WIDTH - totalButtonWidth) / 2;
    
    // Create a panel behind the buttons
    HWND buttonPanel = CreateWindowEx(
        WS_EX_STATICEDGE,
        "STATIC", "",
        WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME,
        startX - 20, buttonY - 10, 
        totalButtonWidth + 40, buttonHeight + 20,
        hwnd, (HMENU)ID_GAME_TAB,
        GetModuleHandle(NULL), NULL
    );
    (void)buttonPanel; // Suppress unused variable warning
    
    // Generate Puzzle button
    HWND generateBtn = CreateWindowEx(
        WS_EX_STATICEDGE,
        "BUTTON", "Generate Puzzle",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT,
        startX, buttonY, buttonWidth, buttonHeight,
        hwnd, (HMENU)ID_GENERATE, GetModuleHandle(NULL), NULL);
    SendMessage(generateBtn, WM_SETFONT, (WPARAM)buttonFont, MAKELPARAM(TRUE, 0));
    
    // Check Solution button
    HWND checkBtn = CreateWindowEx(
        WS_EX_STATICEDGE,
        "BUTTON", "Check Solution",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT,
        startX + buttonWidth + buttonSpacing, buttonY, buttonWidth, buttonHeight,
        hwnd, (HMENU)ID_CHECK, GetModuleHandle(NULL), NULL);
    SendMessage(checkBtn, WM_SETFONT, (WPARAM)buttonFont, MAKELPARAM(TRUE, 0));
    
    // Get Hint button
    HWND hintBtn = CreateWindowEx(
        WS_EX_STATICEDGE,
        "BUTTON", "Get Hint",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT,
        startX + 2 * (buttonWidth + buttonSpacing), buttonY, buttonWidth, buttonHeight,
        hwnd, (HMENU)ID_HINTS, GetModuleHandle(NULL), NULL);
    SendMessage(hintBtn, WM_SETFONT, (WPARAM)buttonFont, MAKELPARAM(TRUE, 0));
    
    // Initially hide game tab controls
    ShowGameTab(hwnd, FALSE);
}

void GenerateSudokuPuzzle(int puzzle[SIZE][SIZE], int solution[SIZE][SIZE]) {
    // Initialize random seed
    srand((unsigned int)time(NULL));
    
    // Generate a complete valid sudoku solution
    if (generate_complete_sudoku(solution)) {
        // Create puzzle by removing numbers (medium difficulty = 1)
        create_puzzle_from_solution(solution, puzzle, 1);
    } else {
        // Fallback to example puzzle if generation fails
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
        
        // Copy example to puzzle
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                puzzle[i][j] = example[i][j];
                solution[i][j] = example[i][j];
            }
        }
        
        // Solve to get the complete solution
        solve_sudoku(solution);
    }
}

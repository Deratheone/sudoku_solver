#include <windows.h>
#include <stdio.h>
#include "gui.h"
#include "sudoku.h"

// Application entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Unused parameters (required by WinMain signature)
    (void)hPrevInstance;
    (void)lpCmdLine;
    
    // Define our window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;        // Function that handles messages
    wc.hInstance = hInstance;           // This application instance
    wc.lpszClassName = "SudokuSolver";  // Class name for our window
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // Background color
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Default arrow cursor
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);     // Default application icon
    
    // Register the window class with Windows
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Failed to register window class!", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
      // Create the main window with a fixed size (non-resizable)
    HWND hwnd = CreateWindow(
        "SudokuSolver",              // Window class name
        "Sudoku Solver - C Project", // Window title
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,  // Fixed size window style
        (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2,  // Center horizontally 
        (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2, // Center vertically
        WINDOW_WIDTH, WINDOW_HEIGHT,   // Window size
        NULL,        // No parent window
        NULL,        // No menu
        hInstance,   // Application instance
        NULL         // No additional data
    );
    
    // Check if window creation succeeded
    if (!hwnd) {
        MessageBox(NULL, "Failed to create window!", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    // Make the window visible
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Print welcome message to console (if running from command line)
    printf("Sudoku Solver Started!\n");
    printf("Instructions:\n");
    printf("1. Enter numbers (1-9) in the grid cells\n");
    printf("2. Leave cells empty for unknowns\n");
    printf("3. Click 'Solve Puzzle' to find the solution\n");
    printf("4. Use 'Clear Grid' to start over\n\n");
    
    // Main message loop - keeps the application running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);  // Translate keyboard input
        DispatchMessage(&msg);   // Send message to WindowProc
    }
    
    return (int)msg.wParam;
}

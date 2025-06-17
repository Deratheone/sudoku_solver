# Sudoku Solver with GUI

A C-based Sudoku solver with a graphical user interface that lets you enter puzzles and find solutions efficiently.

## Features

- **Interactive GUI**: Easy-to-use 9x9 grid for inputting puzzles
- **Fast Solving**: Uses backtracking algorithm for quick solutions
- **Input Validation**: Only accepts valid numbers (1-9)
- **Visual Feedback**: Clear separation of 3x3 blocks
- **User-Friendly**: Simple buttons for solve, clear, and exit

## How to Build

### Prerequisites
- GCC compiler (MinGW recommended for Windows)
- Windows operating system

### Installation

**If you are on Windows and do not have `make` installed:**
- You can use the provided PowerShell script:
  ```powershell
  .\build.ps1
  ```
- Or, compile directly with GCC:
  ```powershell
  gcc -Wall -Wextra -std=c99 -O2 -o sudoku_solver.exe src/main.c src/sudoku.c src/gui.c -lgdi32 -luser32 -lkernel32
  ```

**If you have `make` installed (Linux/macOS or Windows with MinGW/MSYS2):**
- Compile the project:
  ```sh
  make
  ```
- Or for quick build:
  ```sh
  make quick
  ```

## How to Use

1. **Run the program**: Double-click `sudoku_solver.exe` or run `make run`
2. **Enter puzzle**: Click on cells and enter numbers 1-9
3. **Solve**: Click "Solve Puzzle" button
4. **Clear**: Use "Clear Grid" to start over
5. **Exit**: Close window or click "Exit" button

## Example Puzzle

Try entering this puzzle:
```
5 3 _ | _ 7 _ | _ _ _
6 _ _ | 1 9 5 | _ _ _
_ 9 8 | _ _ _ | _ 6 _
------+-------+------
8 _ _ | _ 6 _ | _ _ 3
4 _ _ | 8 _ 3 | _ _ 1
7 _ _ | _ 2 _ | _ _ 6
------+-------+------
_ 6 _ | _ _ _ | 2 8 _
_ _ _ | 4 1 9 | _ _ 5
_ _ _ | _ 8 _ | _ 7 9
```

## Algorithm

Uses **backtracking** algorithm:
1. Find empty cell
2. Try numbers 1-9
3. Check if placement is valid (row, column, 3x3 box)
4. If valid, recursively solve rest
5. If no solution, backtrack and try next number

## Project Structure

```
sudoku_solver/
├── src/
│   ├── main.c     # Entry point and window creation
│   ├── sudoku.c   # Solving algorithm implementation
│   ├── sudoku.h   # Sudoku function declarations
│   ├── gui.c      # GUI implementation
│   └── gui.h      # GUI function declarations
├── Makefile       # Build configuration
└── README.md      # This file
```

## Technical Details

- **Language**: C (C99 standard)
- **GUI**: Windows API (native)
- **Algorithm**: Backtracking with constraint checking
- **Time Complexity**: O(9^n) worst case, typically much faster
- **Space Complexity**: O(1) excluding recursion stack

## Contributing

Feel free to submit issues and enhancement requests!

## License

This project is open source. Feel free to use and modify as needed.

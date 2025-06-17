# Sudoku Solver with GUI and Game Mode

A C-based Sudoku solver with a graphical user interface that features both puzzle solving and interactive gameplay with puzzle generation.

## Features

### Solver Tab
- **Interactive GUI**: Easy-to-use 9x9 grid for inputting puzzles
- **Fast Solving**: Uses backtracking algorithm for quick solutions
- **Input Validation**: Only accepts valid numbers (1-9) and checks for duplicates
- **Visual Feedback**: Clear separation of 3x3 blocks
- **Example Puzzles**: Load sample puzzles to test the solver

### Game Tab
- **Puzzle Generation**: Create random, solvable Sudoku puzzles
- **Solution Checking**: Verify if your completed puzzle is correct
- **Hint System**: Get up to 3 hints per puzzle to help you solve
- **Difficulty Levels**: Medium difficulty puzzles with proper number of clues

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
  gcc -Wall -Wextra -std=c99 -O2 -o sudoku_solver.exe src/main.c src/sudoku.c src/gui.c -lgdi32 -luser32 -lkernel32 -lcomctl32
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

### Solver Tab
1. **Run the program**: Double-click `sudoku_solver.exe` or run `make run`
2. **Enter puzzle**: Click on cells and enter numbers 1-9 in the Solver tab
3. **Solve**: Click "Solve Puzzle" button to find the solution
4. **Clear**: Use "Clear Grid" to start over
5. **Load Example**: Click "Load Example" to try a sample puzzle

### Game Tab  
1. **Generate Puzzle**: Click "Generate Puzzle" to create a new random puzzle
2. **Play**: Fill in the empty cells with numbers 1-9
3. **Check Solution**: Click "Check Solution" when you think you've solved it
4. **Get Hints**: Use "Get Hint" button (3 hints available per puzzle)
5. **Switch Tabs**: Click between "Solver" and "Game" tabs at the top

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

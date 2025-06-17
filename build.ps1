# Simple PowerShell build script for Sudoku Solver
# This script makes it easy to build the project without using make

Write-Host "===== Building Sudoku Solver =====" -ForegroundColor Green
gcc -Wall -Wextra -std=c99 -O2 -o sudoku_solver.exe src/main.c src/sudoku.c src/gui.c -lgdi32 -luser32 -lkernel32 -lcomctl32

if ($LASTEXITCODE -eq 0) {
    Write-Host "===== Build Successful! =====" -ForegroundColor Green
    Write-Host "Run the program with: .\sudoku_solver.exe" -ForegroundColor Cyan
    
    $run = Read-Host "Do you want to run the program now? (y/n)"
    if ($run -eq "y" -or $run -eq "Y") {
        Write-Host "Starting Sudoku Solver..." -ForegroundColor Yellow
        .\sudoku_solver.exe
    }
} else {
    Write-Host "===== Build Failed! =====" -ForegroundColor Red
}

# Sudoku Solver Makefile
# Compatible with both make and direct PowerShell/CMD commands

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LIBS = -lgdi32 -luser32 -lkernel32

# Directories and files
SRCDIR = src
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/sudoku.c $(SRCDIR)/gui.c
TARGET = sudoku_solver.exe

# Default target - builds the executable
all: $(TARGET)
	@echo "Build complete! Run with: $(TARGET)"

# Build the executable
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

# Clean build files
clean:
	-if exist $(TARGET) del $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Quick build for PowerShell users
quick:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)
	@echo "Build complete! Run with: .\$(TARGET)"

# Install MinGW if needed
install-help:
	@echo "If you don't have GCC installed:"
	@echo "1. Install via winget: winget install mingw"
	@echo "2. Or download from: https://www.mingw-w64.org/"
	@echo "3. Add to PATH: C:\MinGW\bin"

# Help - show available commands and manual compilation
help:
	@echo "===== Sudoku Solver Build System ====="
	@echo ""
	@echo "For GNU Make users:"
	@echo "  make all    - Build the project (default)"
	@echo "  make clean  - Remove executable"
	@echo "  make run    - Build and run"
	@echo "  make quick  - Quick build"
	@echo ""
	@echo "For PowerShell users:"
	@echo "  .\build.ps1 - Easy build script (recommended)"
	@echo ""
	@echo "Direct PowerShell command:"
	@echo "  gcc -Wall -Wextra -std=c99 -O2 -o sudoku_solver.exe src/main.c src/sudoku.c src/gui.c -lgdi32 -luser32 -lkernel32"
	@echo ""
	@echo "To run the program:"
	@echo "  .\sudoku_solver.exe"
	@echo ""
	@echo "Need GCC? Run: make install-help"

.PHONY: all clean run quick install-help help

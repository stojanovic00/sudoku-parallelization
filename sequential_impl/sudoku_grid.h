#ifndef SEQUENTIAL_IMPL_SUDOKU_GRID_H
#define SEQUENTIAL_IMPL_SUDOKU_GRID_H

#include <stdbool.h>
#include <malloc.h>
#include <stdbool.h>
#include "math.h"

#define SUDOKU_SIZE 9
#define ALL_POSSIBILITIES_VAL 0b111111111 //one place for each num from 1 to 9

typedef struct {
    int value;
    unsigned possibilities;
} Cell;

typedef Cell* CellArray;
typedef CellArray* SudokuGrid;


SudokuGrid loadSudokuGrid(const char* file_path);
bool storeSudokuGrid(SudokuGrid grid, char* file_path);
SudokuGrid copySudokuGrid(SudokuGrid original);
bool sudokuGridsMatching(SudokuGrid grid1, SudokuGrid grid2);
void freeSudokuGrid(SudokuGrid grid);
void printSudokuGrid(SudokuGrid grid);
bool solved(SudokuGrid grid);

#endif //SEQUENTIAL_IMPL_SUDOKU_GRID_H

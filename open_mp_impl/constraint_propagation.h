#ifndef SEQUENTIAL_IMPL_CONSTRAINT_PROPAGATION_H
#define SEQUENTIAL_IMPL_CONSTRAINT_PROPAGATION_H
#include "sudoku_grid.h"

unsigned countOnes(unsigned binNumber);
void findOptimalCandidate(SudokuGrid grid, int* row, int* col);
void extractPossibilities(unsigned binPossibilities, int** result, int* result_length);
int possibility_convertToBin(int number);
int possibility_convertToDec(unsigned bin_possibility);
unsigned subtractPossibility(unsigned possibilities, unsigned possibility);
bool propagateRulesForCell(SudokuGrid grid, int row, int col);
int getCommonForbiddenForUnit(SudokuGrid grid, int row, int col);

#endif //SEQUENTIAL_IMPL_CONSTRAINT_PROPAGATION_H

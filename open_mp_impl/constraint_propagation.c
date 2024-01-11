#include "constraint_propagation.h"

//Counts 1s occurrences in bin number
unsigned countOnes(unsigned binNumber) {
    unsigned count = 0;
    while (binNumber) {
        // Increment count if the least significant bit is set
        count += binNumber & 1;
        // Right-shift the binNumber to check the next bit
        binNumber >>= 1;
    }
    return count;
}

//Finds cell that has least possibilities
//row and col are return values
void findOptimalCandidate(SudokuGrid grid, int* row, int* col){
    *row = -1, *col = -1;
    unsigned min_count = ALL_POSSIBILITIES_VAL;
    for(int i =0; i < SUDOKU_SIZE; i++){
        for(int j =0; j < SUDOKU_SIZE; j++){
            unsigned possibilities_num = countOnes(grid[i][j].possibilities);
            if( possibilities_num >= 2 && possibilities_num < min_count){
                *row = i;
                *col = j;
                min_count = possibilities_num;
            }
        }
    }
}

//Parses binary number and extracts possible values in decimal representation
void extractPossibilities(unsigned binPossibilities, int** result, int* result_length){
    unsigned result_size = countOnes(binPossibilities);
    *result = (int*) malloc(sizeof(int) * result_size);

    int number = 1;
    int index = 0;
    while (binPossibilities) {
        if (binPossibilities & 1) {
            (*result)[index++] = number;
        }

        binPossibilities >>= 1;
        number++;
    }

    *result_length = index;
}

int possibility_convertToBin(int number){
    int result = 1;
    for(int i = 1; i < number;i++){
        result <<= 1;
    }
    return result;
}

int possibility_convertToDec(unsigned bin_possibility){
    int number = 0;
    while(bin_possibility){
        bin_possibility >>= 1;
        number++;
    }
    return number;
}

//Both parameters have binary representation
unsigned subtractPossibility(unsigned possibilities, unsigned possibility){
    return possibilities & ~possibility;
}

//RULE 1 - removes value of cell from all possibilities of cells in its units
bool propagateRulesForCell(SudokuGrid grid, int row, int col){

    //Check for contradiction horizontal
    for(int i =0; i < SUDOKU_SIZE;i++){
        if( i == col) { continue;}
        if(grid[row][i].value == grid[row][col].value){
            return false;
        }
    }

    //Check for contradiction vertical
    for(int i =0; i < SUDOKU_SIZE;i++){
        if( i == row) { continue;}
        if(grid[i][col].value == grid[row][col].value){
            return false;
        }
    }

    //Check for contradiction box
    int sudokuSizeRoot = (int) sqrt(SUDOKU_SIZE);
    int horizontal_start = (row / sudokuSizeRoot) * sudokuSizeRoot;
    int vertical_start = (col / sudokuSizeRoot) * sudokuSizeRoot;
    for(int i = horizontal_start; i < horizontal_start + sudokuSizeRoot; i++){
        for(int j = vertical_start; j < vertical_start + sudokuSizeRoot; j++){
            if( i == row && j == col) { continue;}
            if(grid[i][j].value == grid[row][col].value){
                return false;
            }
        }
    }

    //Remove from horizontal unit
    for(int i =0; i < SUDOKU_SIZE;i++){
        grid[row][i].possibilities = subtractPossibility(grid[row][i].possibilities,
                                                         possibility_convertToBin(grid[row][col].value));
    }

    //Remove from vertical unit
    for(int i =0; i < SUDOKU_SIZE;i++){
        grid[i][col].possibilities = subtractPossibility(grid[i][col].possibilities,
                                                         possibility_convertToBin(grid[row][col].value));
    }
    //Remove from box unit
    for(int i = horizontal_start; i < horizontal_start + sudokuSizeRoot; i++){
        for(int j = vertical_start; j < vertical_start + sudokuSizeRoot; j++){
            grid[i][j].possibilities = subtractPossibility(grid[i][j].possibilities,
                                                           possibility_convertToBin(grid[row][col].value));
        }
    }

    return true;
}


//By doing ~ you get all forbidden ones for cell
//Combining all forbidden of one unit with & you get common forbidden for unit
//After that you see which of forbidden, are possible for current column by doing &
//of all forbidden and possible of that column
//If you get just one possible return it
int getCommonForbiddenForUnit(SudokuGrid grid, int row, int col){
    if(grid[row][col].value != 0){
        return 0;
    }
    //Horizontal
    unsigned result = ALL_POSSIBILITIES_VAL; //Will be gradually filtrated
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(col == i) { continue; }
        result = result & (~grid[row][i].possibilities);
    }

    result = result & grid[row][col].possibilities;
    if(countOnes(result) == 1){
        return possibility_convertToDec(result);
    }

    //Vertical
    result = ALL_POSSIBILITIES_VAL;
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(row == i) { continue; }
        result = result & (~grid[i][col].possibilities);
    }

    result = result & grid[row][col].possibilities;
    if(countOnes(result) == 1){
        return possibility_convertToDec(result);
    }

    //Box
    result = ALL_POSSIBILITIES_VAL;
    int sudokuSizeRoot = (int) sqrt(SUDOKU_SIZE);
    int horizontal_start = (row / sudokuSizeRoot) * sudokuSizeRoot;
    int vertical_start = (col / sudokuSizeRoot) * sudokuSizeRoot;
    for(int i = horizontal_start; i < horizontal_start + sudokuSizeRoot; i++){
        for(int j = vertical_start; j < vertical_start + sudokuSizeRoot; j++){
            if(row == i && col == j) { continue; }
            unsigned forbidden =  ~grid[i][j].possibilities;
            result = result & forbidden;
        }
    }
    result = result & grid[row][col].possibilities;
    if(countOnes(result) == 1){
        return possibility_convertToDec(result);
    }

    return 0;
}

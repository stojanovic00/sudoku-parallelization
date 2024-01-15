//By doing ~ you get all forbidden ones for cell
//Combining all forbidden of one unit with & you get common forbidden for unit
//After that you see which of forbidden, are possible for current column by doing &
//of all forbidden and possible of that column
//If you get just one possible return it
int getCommonForbiddenForUnit(SudokuGrid grid, int row, int col){
    if(grid[row][col].value != 0){
        return 0;
    }

    //. . .
    //  Na sličan način propagiranje se vrši i po vrstama i kolonama
    //  (nije prikazano zbog preglednosti)
    //. . .

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


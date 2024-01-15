bool propagateRulesForCell(SudokuGrid grid, int row, int col){

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

    //Remove from box unit
    for(int i = horizontal_start; i < horizontal_start + sudokuSizeRoot; i++){
        for(int j = vertical_start; j < vertical_start + sudokuSizeRoot; j++){
            grid[i][j].possibilities = subtractPossibility(grid[i][j].possibilities,
                                                           possibility_convertToBin(grid[row][col].value));
        }
    }

    //. . .
    //  Na sličan način propagiranje se vrši i po vrstama i kolonama
    //  (nije prikazano zbog preglednosti)
    //. . .

    return true;
}


#include "sudoku_grid.h"


SudokuGrid loadSudokuGrid(const char* file_path){
    SudokuGrid grid = (SudokuGrid) malloc(sizeof(CellArray ) * SUDOKU_SIZE);
    if (grid == NULL) {
        fprintf(stderr, "Memory allocation failed for sudoku grid\n");
        return NULL;
    }

    //Open CSV csv_file
    FILE *csv_file = fopen(file_path, "r");
    if (csv_file == NULL) {
        perror("Error opening source csv_file");
        printf("SOURCE FILE: %s", file_path);
        return NULL;
    }

    //Init grid rows
    for(int i = 0; i < SUDOKU_SIZE; i++) {
        //Load CSV line

        int csv_line_values[SUDOKU_SIZE];
       if(SUDOKU_SIZE == 9) {
           fscanf(csv_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d", &csv_line_values[0], &csv_line_values[1], &csv_line_values[2],
                  &csv_line_values[3], &csv_line_values[4], &csv_line_values[5],
                  &csv_line_values[6], &csv_line_values[7], &csv_line_values[8]);
       } else if(SUDOKU_SIZE == 16){
           fscanf(csv_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &csv_line_values[0], &csv_line_values[1], &csv_line_values[2],
                  &csv_line_values[3], &csv_line_values[4], &csv_line_values[5],
                  &csv_line_values[6], &csv_line_values[7], &csv_line_values[8],
                  &csv_line_values[9], &csv_line_values[10], &csv_line_values[11],
                  &csv_line_values[12], &csv_line_values[13], &csv_line_values[14], &csv_line_values[15]);
       } else{
           printf("WRONG SUDOKU SIZE");
           return NULL;
       }

        CellArray row = (CellArray) malloc(sizeof(Cell) * SUDOKU_SIZE);
        if (row == NULL) {
            fprintf(stderr, "Memory allocation failed for grid row %d\n", i);
            //Free already allocated rows
            for (int j = 0; j < i;j++) {
                free(grid[j]);
            }
        }
        //Init all cells in row
        for(int k = 0; k < SUDOKU_SIZE;k++){
            int value = csv_line_values[k];
            int possibilities =  ALL_POSSIBILITIES_VAL;
            if(value != 0){
                possibilities = 0;
            }
            row[k] = (Cell) {value, possibilities};
        }

        grid[i] = row;
    }

    return grid;
}


SudokuGrid copySudokuGrid(SudokuGrid original) {
    SudokuGrid copy = malloc(SUDOKU_SIZE * sizeof(CellArray));

    for (int i = 0; i < SUDOKU_SIZE; i++) {
        copy[i] = malloc(SUDOKU_SIZE * sizeof(Cell));
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            copy[i][j] = original[i][j];
        }
    }

    return copy;
}

bool sudokuGridsMatching(SudokuGrid grid1, SudokuGrid grid2) {
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (grid1[i][j].value != grid2[i][j].value || grid1[i][j].possibilities != grid2[i][j].possibilities) {
                // If values or possibilities are not matching, return false
                return false;
            }
        }
    }
    // All cells are matching
    return true;
}



bool storeSudokuGrid(SudokuGrid grid, char* file_path){
    FILE *csv_output_file = fopen(file_path, "w");
    if (csv_output_file == NULL) {
        perror("Error opening output csv_output_file");
        return false;
    }

    for(int i = 0; i < SUDOKU_SIZE;i++){
        CellArray row = grid[i];
        if(SUDOKU_SIZE == 9){
            fprintf(csv_output_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",row[0].value,row[1].value,row[2].value,row[3].value,
                    row[4].value,row[5].value,row[6].value,row[7].value,row[8].value);
        } else if(SUDOKU_SIZE == 16){
            fprintf(csv_output_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",row[0].value,row[1].value,row[2].value,row[3].value,
                    row[4].value,row[5].value,row[6].value,row[7].value,row[8].value,
                    row[9].value,row[10].value,row[11].value,row[12].value,row[13].value,row[14].value,row[15].value);
        } else{
            printf("WRONG SUDOKU SIZE\n");
            return false;
        }
    }
    fclose(csv_output_file);
    return true;
}

void freeSudokuGrid(SudokuGrid grid) {
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        free(grid[i]);
    }

    free(grid);
}


void printSudokuGrid(SudokuGrid grid){
    int sudokuSizeRoot = (int) sqrt(SUDOKU_SIZE);

    //Print box border
    for(int k = 0; k < SUDOKU_SIZE * 2 + (SUDOKU_SIZE/sudokuSizeRoot)*2 + 1; k++){
        printf("--");
    }
    printf("\n");

    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(j == 0){
                printf("| %2d ", grid[i][j].value);
            } else {
                printf("%2d ", grid[i][j].value);
            }
            if((j + 1) % sudokuSizeRoot == 0){
                printf("| ");
            }
        }

        printf("\n");
        //Print box border
        if((i + 1) % sudokuSizeRoot == 0){
            for(int k = 0; k < SUDOKU_SIZE * 2 + (SUDOKU_SIZE/sudokuSizeRoot)*2 + 1; k++){
                printf("--");
            }
            printf("\n");
        }
    }
}

bool solved(SudokuGrid grid){
    for(int i =0; i < SUDOKU_SIZE; i++){
        for(int j =0; j < SUDOKU_SIZE; j++){
            if(grid[i][j].possibilities != 0){
                return false;
            }
        }
    }

    return true;
}

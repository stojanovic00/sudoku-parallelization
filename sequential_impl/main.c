#include <malloc.h>
#include "stdio.h"
#include "math.h"
#include "stdbool.h"


#define SUDOKU_SIZE 9
#define ALL_POSSIBILITIES_VAL 0b111111111 //one place for each num from 1 to 9

typedef struct {
    int value;
    int possibilities;
} Cell;

typedef Cell* CellArray;
typedef CellArray* SudokuGrid;

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
        int csv_line_values[9];
        fscanf(csv_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d", &csv_line_values[0], &csv_line_values[1], &csv_line_values[2],
               &csv_line_values[3], &csv_line_values[4], &csv_line_values[5],
               &csv_line_values[6], &csv_line_values[7], &csv_line_values[8]);

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
            row[k] = (Cell) {csv_line_values[k], ALL_POSSIBILITIES_VAL};
        }

        grid[i] = row;
    }

    return grid;
}


bool storeSudokuGrid(SudokuGrid grid, char* file_path){
    FILE *csv_output_file = fopen(file_path, "w");
    if (csv_output_file == NULL) {
        perror("Error opening output csv_output_file");
        return false;
    }

    for(int i = 0; i < SUDOKU_SIZE;i++){
        CellArray row = grid[i];
        fprintf(csv_output_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",row[0].value,row[1].value,row[2].value,row[3].value,
                row[4].value,row[5].value,row[6].value,row[7].value,row[8].value);
    }

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
        printf("-");
    }
    printf("\n");

   for(int i = 0; i < SUDOKU_SIZE; i++){
       for(int j = 0; j < SUDOKU_SIZE; j++){
           if(j == 0){
               printf("| %d ", grid[i][j].value);
           } else {
               printf("%d ", grid[i][j].value);
           }
           if((j + 1) % sudokuSizeRoot == 0){
               printf("| ");
           }
       }

       printf("\n");
       //Print box border
       if((i + 1) % sudokuSizeRoot == 0){
           for(int k = 0; k < SUDOKU_SIZE * 2 + (SUDOKU_SIZE/sudokuSizeRoot)*2 + 1; k++){
               printf("-");
           }
           printf("\n");
       }
   }
}

int main(int argc, char** argv){
    if(argc != 3){
        printf("Usage: sequential_solve <input_file> <output_file>");
        return 1;
    }

    SudokuGrid grid = loadSudokuGrid(argv[1]);
    if(grid == NULL){
        return 1;
    }

    printSudokuGrid(grid);

    bool stored = storeSudokuGrid(grid, argv[2]);
    if(!stored){
        return 1;
    }

    freeSudokuGrid(grid);
    return 0;
}
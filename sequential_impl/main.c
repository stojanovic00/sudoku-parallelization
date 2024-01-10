#include <malloc.h>
#include "stdio.h"
#include "math.h"
#include "stdbool.h"


#define SUDOKU_SIZE 9
#define ALL_POSSIBILITIES_VAL 0b111111111 //one place for each num from 1 to 9

typedef struct {
    int value;
    unsigned possibilities;
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


//Checks if sudoku is solved
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

//Counts 1 occurences in bin number
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
            if( possibilities_num > 2 && possibilities_num < min_count){
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



//TODO
SudokuGrid search(SudokuGrid grid){
    //Finished earlier
     if(grid == NULL) {
         return NULL;
    }
    //Solved
    if(solved(grid)){
        return grid;
    }

    int row, col;
    findOptimalCandidate(grid,&row, &col);
    printf("OPTIMAL CANDIDATE: (%d,%d)\n", row, col);
    int* possibilities = NULL;
    int possibilities_size;
    extractPossibilities(grid[row][col].possibilities, &possibilities, &possibilities_size);

    //Depth first search for all possibilities of that cell
    for(int i =0; i < possibilities_size; i++){

    }



    return NULL;
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

SudokuGrid solve(SudokuGrid grid){
    //CONSTRAINT PROPAGATION

    //Initial propagation of rules for cues
    for(int i = 0; i < SUDOKU_SIZE;i++){
        for(int j = 0; j < SUDOKU_SIZE;j++){
           if(grid[i][j].value != 0){
               propagateRulesForCell(grid, i, j);
           }
        }
    }

    //Deep copy
    SudokuGrid previous_grid = copySudokuGrid(grid);
    while(true){
        //Apply rules
        for(int i = 0; i < SUDOKU_SIZE;i++){
            for(int j = 0; j < SUDOKU_SIZE;j++){
                //Find singletons
                if(countOnes(grid[i][j].possibilities) == 1){
                    grid[i][j].value = possibility_convertToDec(grid[i][j].possibilities);
                    grid[i][j].possibilities = 0;
                    propagateRulesForCell(grid, i, j);
                    continue;
                }

                //RULE 2
                int forbidden = getCommonForbiddenForUnit(grid, i, j);
                if(forbidden != 0){
                    grid[i][j].value = forbidden;
                    grid[i][j].possibilities = 0;
                    propagateRulesForCell(grid, i, j);
                    continue;
                }
            }
        }

        //Now compare, if there is no change break
        if(sudokuGridsMatching(previous_grid, grid)){
            break;
        }
        //Deep copy
        previous_grid = copySudokuGrid(grid);
    }

    if(solved(grid)){
        return grid;
    }

    //SEARCH

    return NULL;
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
    SudokuGrid  solved = solve(grid);

    if(solved == NULL){
        printf("Sudoku can't be solved");
        return 1;
    }
    printSudokuGrid(solved);

    bool stored = storeSudokuGrid(solved, argv[2]);
    if(!stored){
        return 1;
    }

    freeSudokuGrid(grid);
//    freeSudokuGrid(solved);
    return 0;
}
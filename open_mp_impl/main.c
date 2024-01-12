#include <unistd.h>
#include <omp.h>
#include "stdio.h"
#include "stdbool.h"
#include "sudoku_grid.h"
#include "constraint_propagation.h"


SudokuGrid solve(SudokuGrid grid, bool* stopFlag){
    //CONSTRAINT PROPAGATION
    //Initial propagation of rules for cues
    for(int i = 0; i < SUDOKU_SIZE;i++){
        for(int j = 0; j < SUDOKU_SIZE;j++){
            if(grid[i][j].value != 0){
                bool success = propagateRulesForCell(grid, i, j);
                if(!success) {return NULL;}
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
                    bool success = propagateRulesForCell(grid, i, j);
                    if(!success) {return NULL;}
                    continue;
                }

                //RULE 2
                int forbidden = getCommonForbiddenForUnit(grid, i, j);
                if(forbidden != 0){
                    grid[i][j].value = forbidden;
                    grid[i][j].possibilities = 0;
                    bool success = propagateRulesForCell(grid, i, j);
                    if(!success) {return NULL;}
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
    int optimal_i, optimal_j;
    findOptimalCandidate(grid, &optimal_i, &optimal_j);

    int* possibilities;
    int possibilities_length;
    extractPossibilities(grid[optimal_i][optimal_j].possibilities, &possibilities, &possibilities_length);

    SudokuGrid finalResult = NULL;
    //Maximum SUDOKU_SIZE possibilities, so threads also
    #pragma omp parallel num_threads(SUDOKU_SIZE) shared(finalResult, stopFlag)
    {
        #pragma omp for
        for (int i = 0; i < possibilities_length; i++) {
            //Check if some thread found solution
            if (finalResult != NULL || *stopFlag) {
                continue;
            }

            SudokuGrid guessGrid = copySudokuGrid(grid);
            guessGrid[optimal_i][optimal_j].value = possibilities[i];
            guessGrid[optimal_i][optimal_j].possibilities = 0;

            if(finalResult != NULL || *stopFlag){
                continue;
            }
            SudokuGrid result = solve(guessGrid, stopFlag);
            if (result == NULL) {
                //NULL means that this guess consequently violated rules
                continue;
            } else {
                #pragma omp critical
                {
                    *stopFlag = true;
                    finalResult = result;
                }
            }
        }
    }

    return finalResult;
}


int main(int argc, char** argv){
    if(argc != 3){
        printf("Usage: sequential_solve <input_file> <output_file>");
        return 1;
    }

    //Load
    SudokuGrid grid = loadSudokuGrid(argv[1]);
    if(grid == NULL){
        return 1;
    }
//    printf("Original:\n");
//    printSudokuGrid(grid);

// Slows it down even more
//    omp_set_nested(1);
//    omp_set_max_active_levels(10);
    bool stopFlag = false;
    SudokuGrid  solved = solve(grid, &stopFlag);
    if(solved == NULL){
        printf("Sudoku can't be solved");
        return 1;
    }
//    printf("Solved:\n");
//    printSudokuGrid(solved);
    printf("Sudoku solved\n");

    //Store
    bool stored = storeSudokuGrid(solved, argv[2]);
    if(!stored){
        return 1;
    }

    freeSudokuGrid(grid);
    return 0;
}

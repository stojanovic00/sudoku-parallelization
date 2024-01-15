    #pragma omp parallel num_threads(sudokuSizeSqrt) shared(finalResult, stopFlag)
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


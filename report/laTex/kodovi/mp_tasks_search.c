#pragma omp parallel num_threads(sudokuSizeSqrt) shared(finalResult, stopFlag)
{
    #pragma omp single nowait
    for (int i = 0; i < possibilities_length; i++) {
        // Create tasks for each possibility
        #pragma omp task
        {
            // Check if some task found a solution
            if (!(finalResult != NULL || *stopFlag)) {

                SudokuGrid guessGrid = copySudokuGrid(grid);
                guessGrid[optimal_i][optimal_j].value = possibilities[i];
                guessGrid[optimal_i][optimal_j].possibilities = 0;


                SudokuGrid result = solve(guessGrid, stopFlag);

                #pragma omp critical
                {
                    if (result != NULL) {
                        *stopFlag = true;
                        finalResult = result;
                    }
                }
            }
        }
    }
}


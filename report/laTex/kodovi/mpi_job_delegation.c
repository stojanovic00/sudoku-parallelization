//Delegate work
if(possibilities_length > 1){
   for(int i = 1; i < num_proc;i++){
       if(i == rank) { continue;}
       if(availabilities[i]){
           SudokuGrid guessGrid = copySudokuGrid(result);
           guessGrid[optimal_i][optimal_j].value = possibilities[delegated];
           guessGrid[optimal_i][optimal_j].possibilities = 0;
           availabilities[i] =false;

           Cell flattened[SUDOKU_SIZE*SUDOKU_SIZE];
           flattenSudokuGrid(guessGrid, flattened);
           MPI_Send(flattened, SUDOKU_SIZE*SUDOKU_SIZE, cell_type, i, 2, MPI_COMM_WORLD);
           delegated++;
       }
   }
}


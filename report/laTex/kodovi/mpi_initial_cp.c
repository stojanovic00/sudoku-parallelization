// Load and initial constraint propagation
if(rank == 0){
    //Load
    SudokuGrid grid = loadSudokuGrid(argv[1]);

    SudokuGrid result = constraintPropagation(grid);

    if(solved(result)){
        Cell flattened[SUDOKU_SIZE*SUDOKU_SIZE];
        flattenSudokuGrid(result, flattened);
        MPI_Isend(flattened, SUDOKU_SIZE*SUDOKU_SIZE, cell_type, 0, 0, MPI_COMM_WORLD, NULL);
    } else {
        //Start processing with process 1
        //Send job
        Cell flattened[SUDOKU_SIZE*SUDOKU_SIZE];
        flattenSudokuGrid(result, flattened);
        MPI_Send(flattened, SUDOKU_SIZE*SUDOKU_SIZE, cell_type, 1, 2, MPI_COMM_WORLD);
    }
}


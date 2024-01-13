#include <mpi.h>
#include "stdio.h"
#include "stdbool.h"
#include "sudoku_grid.h"
#include "constraint_propagation.h"



typedef struct {
    unsigned rank;
    unsigned status;
} Availability;

SudokuGrid constraintPropagation(SudokuGrid grid){
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

    return grid;
}


// Function to flatten a Sudoku grid
void flattenSudokuGrid(const SudokuGrid input, Cell* flattened) {
    for (int i = 0; i < SUDOKU_SIZE; ++i) {
        for (int j = 0; j < SUDOKU_SIZE; ++j) {
            flattened[i * SUDOKU_SIZE + j].value = input[i][j].value;
            flattened[i * SUDOKU_SIZE + j].possibilities = input[i][j].possibilities;
        }
    }
}

// Function to reconstruct a Sudoku grid
void reconstructSudokuGrid(const Cell* flattened, SudokuGrid output) {
    for (int i = 0; i < SUDOKU_SIZE; ++i) {
        for (int j = 0; j < SUDOKU_SIZE; ++j) {
            output[i][j].value = flattened[i * SUDOKU_SIZE + j].value;
            output[i][j].possibilities = flattened[i * SUDOKU_SIZE + j].possibilities;
        }
    }
}
bool solve(SudokuGrid grid, MPI_Datatype cell_type, MPI_Datatype availability_type, unsigned availabilities[], int num_proc){

    //Update process availabilities
    int message_flag;
    MPI_Status status;
    MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &message_flag, &status);

    // 3 - av change
    if (message_flag) {
        int tag = status.MPI_TAG;

        if (tag == 3) {
            // Receive the incoming message (Sudoku grid)
            MPI_Recv(availabilities, num_proc, MPI_UNSIGNED, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }




    //Solve
    bool found = false;
    SudokuGrid result = constraintPropagation(grid);

    if(solved(result)){
        Cell flattened[SUDOKU_SIZE*SUDOKU_SIZE];
        flattenSudokuGrid(result, flattened);
        MPI_Send(flattened, SUDOKU_SIZE*SUDOKU_SIZE, cell_type, 0, 0, MPI_COMM_WORLD);
        found = true;
        return found;
    } else{
        //SEARCH
        int optimal_i, optimal_j;
        findOptimalCandidate(result, &optimal_i, &optimal_j);

        int* possibilities;
        int possibilities_length;
        extractPossibilities(result[optimal_i][optimal_j].possibilities, &possibilities, &possibilities_length);

        int delegated = 0;
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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

        for(int i = delegated; i < possibilities_length; i++){
            SudokuGrid guessGrid = copySudokuGrid(result);
            guessGrid[optimal_i][optimal_j].value = possibilities[i];
            guessGrid[optimal_i][optimal_j].possibilities = 0;

            found = solve(guessGrid, cell_type, availability_type, availabilities, num_proc);
            if(found){
                break;
            }
        }

    }
    return found;
}



int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    if(argc != 3){
        printf("Usage: sequential_solve <input_file> <output_file>");
        return 1;
    }

    //Defining data types
    MPI_Datatype cell_type;
    MPI_Type_contiguous(2, MPI_UNSIGNED, &cell_type);
    MPI_Type_commit(&cell_type);

    MPI_Datatype availability_type;
    MPI_Type_contiguous(2, MPI_UNSIGNED, &availability_type);
    MPI_Type_commit(&availability_type);

    //Basic
    int rank;
    int num_proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    SudokuGrid finalResult = (SudokuGrid)malloc(SUDOKU_SIZE * sizeof(CellArray));
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        finalResult[i] = (CellArray)malloc(SUDOKU_SIZE * sizeof(Cell));
    }

    SudokuGrid currentGrid = (SudokuGrid)malloc(SUDOKU_SIZE * sizeof(CellArray));
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        currentGrid[i] = (CellArray)malloc(SUDOKU_SIZE * sizeof(Cell));
    }

    unsigned availabilities[SUDOKU_SIZE];
    for(int i = 0; i < SUDOKU_SIZE;i++){
        availabilities[i] = true;
    }


    // Load and initial constraint propagation
    if(rank == 0){
        //Load
        SudokuGrid grid = loadSudokuGrid(argv[1]);

        SudokuGrid result = constraintPropagation(grid);

        if(solved(result)){
            Cell flattened[SUDOKU_SIZE*SUDOKU_SIZE];
            flattenSudokuGrid(result, flattened);
            MPI_Send(flattened, SUDOKU_SIZE*SUDOKU_SIZE, cell_type, 0, 0, MPI_COMM_WORLD);
        } else {
            //Start processing with process 1
            //Send job
            Cell flattened[SUDOKU_SIZE*SUDOKU_SIZE];
            flattenSudokuGrid(result, flattened);
            MPI_Send(flattened, SUDOKU_SIZE*SUDOKU_SIZE, cell_type, 1, 2, MPI_COMM_WORLD);
        }
    }

    while(true){
        if(rank == 0){
            //Check if some process found solution
            MPI_Status recv_status;
            int message_flag;

            // Probe for an incoming message
            MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &message_flag, &recv_status);

            if (message_flag) {
                int tag = recv_status.MPI_TAG;
                if(tag == 0){
                   //Solution found
                    // Receive the incoming message (Sudoku grid)
                    Cell flattened[SUDOKU_SIZE * SUDOKU_SIZE];
                    MPI_Recv(flattened, SUDOKU_SIZE * SUDOKU_SIZE, cell_type, recv_status.MPI_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    reconstructSudokuGrid(flattened, finalResult);

                    // Store it and finish the application
                    bool stored = storeSudokuGrid(finalResult, argv[2]);
                    if (!stored) {
                        printf("Error occurred\n");
                    }

                    printf("Sudoku solved\n");

                    // Signal processes to stop
                    for (int i = 1; i < num_proc; i++) {
                        int stopSignal = 1;
                        MPI_Send(&stopSignal, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                    }
                    break;
                } else if(tag == 1){
                    //Availability change
                    Availability avChange;
                    MPI_Recv(&avChange, 1, availability_type, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    availabilities[avChange.rank] = avChange.status;

                    //Send updated availabilities to everyone
                    for(int i =1; i < num_proc;i++){
                        MPI_Isend(availabilities, num_proc, MPI_UNSIGNED, i, 3, MPI_COMM_WORLD, NULL);
                    }
                }
            }
            } else{
            // Check if there is an incoming message
            int message_flag;
            MPI_Status status;
            MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &message_flag, &status);

            if (message_flag) {
                int tag = status.MPI_TAG;

                if (tag == 2) {
                    // Receive the incoming message (Sudoku grid)
                    Cell flattened[SUDOKU_SIZE * SUDOKU_SIZE];
                    MPI_Recv(flattened, SUDOKU_SIZE * SUDOKU_SIZE, cell_type, status.MPI_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    reconstructSudokuGrid(flattened, currentGrid);

                    //Change proc to not available
                    Availability avChange = {rank, 0};
                    MPI_Send(&avChange, 1, availability_type, 0, 1, MPI_COMM_WORLD);

                    solve(currentGrid, cell_type, availability_type, availabilities, num_proc);

                    //Change proc to available
                    avChange = (Availability) {rank, 1};
                    MPI_Send(&avChange, 1, availability_type, 0, 1, MPI_COMM_WORLD);

                } else if (tag == 1) {
                    // Stop signal received
                    break;
                }
            }
        }
    }

    MPI_Finalize();
    return 0;
}
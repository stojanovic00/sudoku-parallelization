int optimal_i, optimal_j;
findOptimalCandidate(grid, &optimal_i, &optimal_j);

int* possibilities;
int possibilities_length;
extractPossibilities(grid[optimal_i][optimal_j].possibilities, &possibilities, &possibilities_length);

for(int i = 0; i < possibilities_length;i++){
    SudokuGrid guessGrid = copySudokuGrid(grid);
    guessGrid[optimal_i][optimal_j].value = possibilities[i];
    guessGrid[optimal_i][optimal_j].possibilities = 0;

    SudokuGrid result = solve(guessGrid);
    if(result == NULL){
        //NULL means that this guess consequently violated rules
        continue ;
    }else{
        return result;
    }
}


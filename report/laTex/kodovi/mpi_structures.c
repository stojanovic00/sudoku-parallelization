//Defining data types
MPI_Datatype cell_type;
MPI_Type_contiguous(2, MPI_UNSIGNED, &cell_type);
MPI_Type_commit(&cell_type);

MPI_Datatype availability_type;
MPI_Type_contiguous(2, MPI_UNSIGNED, &availability_type);
MPI_Type_commit(&availability_type);

unsigned availabilities[num_proc];
for(int i = 0; i < num_proc;i++){
    availabilities[i] = true;
}




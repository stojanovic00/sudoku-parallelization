// Check if there is an incoming message
int message_flag;
MPI_Status status;
MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &message_flag, &status);

if (message_flag) {
    int tag = status.MPI_TAG;

    if (tag == 2) {
      //... Do something...
    } else if (tag == 1) {
        // Stop signal received
        break;
    }
}


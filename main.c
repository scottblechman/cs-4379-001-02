#include<mpi.h>
#include<stdio.h>
#include<stdlib.h> // rand(), srand()
#include<time.h>

#define RANKS 8
#define DEBUG

int main(int argc, char** argv) {
    int ranks=RANKS;

    int size;    // needed for sending # of processes to MPI
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    //#ifdef DEBUG
	////sleep a certain amount of s (offset based on ranks to demonstrate sync)
    //int offset = rank+1;
	//int s = 0;
	//for(int i = 0; i < 500000000*offset; i++){
	//	s = i;
	//}
    //#endif

    int data = 1;
    MPI_Status status;
    //implement broadcasting
    int step;

    // BROAD RECEIVE

    for(step = ranks>>1; step >= 1; step = step >> 1) {
        if(rank >= step) {
#ifdef DEBUG
            printf("%d: here0 %d, Send? = %d", rank, step, rank-step);
#endif
	        MPI_Send(&data, 1, MPI_INT, rank-step, rank-step, MPI_COMM_WORLD);
        } else if(rank < step) { 
#ifdef DEBUG
            printf("%d: here0 %d, Recv? = %d\n", rank, step, rank+step);
#endif
	        MPI_Recv(&data, 1, MPI_INT, rank+step, rank+step, MPI_COMM_WORLD, &status);
        }
    }   

    // BROADCAST
    
    for(step = 1; step < ranks; step*=2){
        if(rank < step) {
#ifdef DEBUG
            printf("%d: here0 %d, Send? = %d", rank, step, rank-step);
#endif
            MPI_Send(&data, 1, MPI_INT, rank+step, rank+step, MPI_COMM_WORLD);
        } else if(rank < 2*step) {
#ifdef DEBUG
            printf("%d: here0 %d, Recv? = %d\n", rank, step, rank+step);
#endif
            MPI_Recv(&data, 1, MPI_INT, rank-step, rank, MPI_COMM_WORLD, &status);
        }
    }

    return MPI_Finalize(); 
}


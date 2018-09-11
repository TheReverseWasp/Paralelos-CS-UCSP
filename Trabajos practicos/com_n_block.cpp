#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]) {
  int count[100]{0};
  int comm_sz;
  int my_rank;
  int acum;
  MPI_Request r[2];
  MPI_Status s[2];
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  if(my_rank==0){
    for (int i = 0; i < 100; i++) {
      count[i] = i;
    }
    MPI_Isend(count,100,MPI_INT,1,0,MPI_COMM_WORLD, &r[0]);
  }
  else{
    MPI_Irecv(count,100,MPI_INT,0,0,MPI_COMM_WORLD, &r[1]);
    for (int i = 0; i < 100; i++) {
      acum += count[i];
    }
    printf("%d\n", acum);
  }
  MPI_Finalize();
  printf("\n");
  return 0;
}

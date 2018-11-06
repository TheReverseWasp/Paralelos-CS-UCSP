#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]) {
  int count[100]{0};
  int comm_sz;
  int my_rank;
  int acum;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  if(my_rank==0){
    for (int i = 0; i < 100; i++) {
      count[i] = i;
    }
    MPI_Send(count,100,MPI_INT,1,0,MPI_COMM_WORLD);
  }
  else{
    MPI_Recv(count,100,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    for (int i = 0; i < 100; i++) {
      acum += count[i];
    }
    printf("%d\n", acum);
  }

  MPI_Finalize();
  printf("\n");
  return 0;
}

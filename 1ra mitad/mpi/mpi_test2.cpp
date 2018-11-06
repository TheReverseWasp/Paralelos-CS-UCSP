/*ping pong*/

#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]) {
  int count[1];
  int comm_sz;
  int my_rank;

  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  count[0]=0;
  if(my_rank==0){
    printf("%d ",count[0]);
    MPI_Send(count,1,MPI_INT,1,0,MPI_COMM_WORLD);
    while(count[0]<100){
      MPI_Recv(count,1,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("%d ",count[0]);
      count[0]++;
      MPI_Send(count,1,MPI_INT,1,0,MPI_COMM_WORLD);
    }
  }
  else{
    while(count[0]<100){
      MPI_Recv(count,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("%d ",count[0]);
      if(count[0]>100){
        break;
      }
      count[0]++;
      MPI_Send(count,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();
  printf("\n");
  return 0;
}

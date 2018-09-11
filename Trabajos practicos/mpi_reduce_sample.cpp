#include <iostream>
#include <mpi.h>

#define MAX 500000

using namespace std;

int main(int argc, char const *argv[]) {

  int my_rank,comm_sz;
  int arr[MAX],answer;

  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

  if(my_rank==0){
    for (long i = 0; i < MAX; i++) {
      arr[i]=i%4;
    }

  }
  MPI_Bcast(arr,MAX,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Reduce(arr,&answer,MAX,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

  MPI_Finalize();
  if(my_rank==0){
    printf("%d\n",answer);
  }

  return 0;
}

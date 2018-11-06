#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <algorithm>
#include <vector>
#define sz 20

using namespace std;

int main(int argc, char const *argv[]) {
  int count[sz]{0};
  int comm_sz;
  int my_rank;
  int acum;
  int ini, fin;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  int temp[sz]{0};
  if(my_rank == 0){
    for (int i = 0; i < sz; i++) {
      count[i] = sz - i;
      printf("%d\n", count[i]);
    }
    printf("\n");
    MPI_Bcast(count,sz,MPI_INT,0,MPI_COMM_WORLD);
    for (size_t i = 1; i < comm_sz; i++) {
      MPI_Recv(temp,sz,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      for (int j = 0; j < (sz / comm_sz); j++) {
        count[(i - 1) * (sz / comm_sz) + j] = temp[(i - 1) * (sz / comm_sz) + j];
      }
    }
  }
  else{
    sort(count + (sz / comm_sz) * (my_rank - 1), (sz / comm_sz));
    MPI_Send(count,sz,MPI_INT,0,0,MPI_COMM_WORLD);
  }
  int total = comm_sz / 2 - 1;
  int first_ini;
  if (my_rank = 0) {
    for (int i = 0; i < comm_sz; i++) {
      if(i % 2 == 0){
        total++;
        first_ini = 0;
      }
      else{
        total--;
        first_ini = sz / comm_sz;
      }
      for (int j = 1; j <= total; j++) {
        MPI_Send(count,sz,MPI_INT,j,0,MPI_COMM_WORLD);
        ///por agregar
      }
    }
  }
  else{
    MPI_Recv(temp,sz,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    ///por agregar
  }
  MPI_Finalize();
  printf("\n");
  return 0;
}

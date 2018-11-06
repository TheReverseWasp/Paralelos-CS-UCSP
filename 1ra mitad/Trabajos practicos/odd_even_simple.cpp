#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <algorithm>
#include <vector>
#define sz 21

using namespace std;

void printarray(int *count, int szu){
  for (int i = 0; i < szu; i++) {
    printf("%d ", count[i]);
  }
  printf("\n");
}

int main(int argc, char const *argv[]) {
  int count[sz]{0};
  int comm_sz;
  int my_rank;
  int acum;
  int ini, fin;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  int temp[2]{0};
  if(my_rank == 0){
    for (int i = 0; i < sz; i++) {
      count[i] = sz - i;
    }
    printarray(count, sz);
    for (int k = 0; k < sz; k++) {
      for (int i = k % 2, j = 1; j < comm_sz; i+=2, j++) {
        temp[0] = count[i];
        temp[1] = count[i + 1];
        MPI_Send(temp,2,MPI_INT,j,0,MPI_COMM_WORLD);
      }
      for (int i = k % 2, j = 1; j < comm_sz; i+=2, j++) {
        MPI_Recv(temp,2,MPI_INT,j,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        count[i] = temp[0];
        count[i+1] = temp[1];
      }
      printarray(count, sz);
    }
  }
  else{
    for (int i = 0; i < sz; i++) {
      MPI_Recv(temp,2,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      if(temp[0] > temp[1]){
        swap(temp[0], temp[1]);
      }
      MPI_Send(temp,2,MPI_INT,0,0,MPI_COMM_WORLD);
    }
  }
  MPI_Finalize();
  return 0;
}

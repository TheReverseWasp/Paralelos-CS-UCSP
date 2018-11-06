#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <algorithm>
#include <vector>
#include <iostream>

#define MAX 20
#define ll long long

using namespace std;

vector <ll> v (MAX, 1);
vector <ll> answeranswer (MAX, 0);

void multiply(ll _1[MAX][MAX], int i){
  for (int j = 0; j < MAX; j++){
    answeranswer[i] += _1[i][j] * v[j];
  }
}

void print_m(ll _1[MAX][MAX]) {
  for (int i = 0; i < MAX; i++) {
    for (int j = 0; j < MAX; j++) {
      printf("%lld ", _1[i][j]);
    }
    printf("\n");
  }
}

void print_v(vector <ll> t) {
  for (int i = 0; i < t.size(); i++) {
    cout << t[i] <<endl;
  }
}


int main(int argc, char const *argv[]) {
  ll a[MAX][MAX];
  //ll b[MAX][MAX];
  //ll c[MAX][MAX];
  int comm_sz;
  int my_rank;
  int acum;
  int ini, fin;
  int temp;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  if(my_rank == 0){
    for (int i = 0; i < MAX; i++) {
      for (int j = 0; j < MAX; j++) {
        /*if (i == j) {
          b[i][j] = 1;
        }
        else {
          b[i][j] = 0;
        }*/
        a[i][j] = 3;
        //c[i][j] = 0;
      }
    }
    print_m(a);
    cout << endl;
    //print_v(answeranswer);
    //print_v(v);
    //print_m(b);
    //cout << endl;
  }
  for (size_t i = 0; i < MAX; i++) {
    MPI_Bcast(&a[i], MAX, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    //MPI_Bcast(b[i], MAX, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Bcast(c[i], MAX, MPI_INT, 0, MPI_COMM_WORLD);
  }
  if(my_rank == 0){
    for (int i = 1; i <= MAX; i++) {
      //MPI_Recv(c[i - 1], MAX, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&answeranswer[i-1], 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    print_v(answeranswer);
  }
  else{
    multiply(a, my_rank - 1);
    //MPI_Send(c[my_rank - 1], MAX, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&answeranswer[my_rank-1], 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}

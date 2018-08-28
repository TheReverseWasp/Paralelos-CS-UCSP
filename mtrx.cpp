#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include <math.h>

#define MAX 100
#define ll long long
using namespace std;


void multiply(ll _1[MAX][MAX], ll _2[MAX][MAX], ll _3[MAX][MAX], int N)
{
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            _3[i][j] = 0;
            for (int k = 0; k < N; k++){
                _3[i][j] += _1[i][k]*_2[k][j];
            }
        }
    }
}
void sixloop(ll _1[MAX][MAX], ll _2[MAX][MAX], ll _3[MAX][MAX], int b, int N){
  for(int i0=0; i0<N; i0+=b ){
    for(int j0=0; j0<N; j0+=b){
      for(int k0=0; k0<N; k0+=b){
        for(int i=i0; i<min(i0+b-1,N); ++i){
          for(int j=j0; j<min(j0+b-1,N); ++j){
            for(int k=k0; k<min(k0+b-1,N); ++k){
              _3[i][j]+=_1[i][k]*_2[k][j];
            }
          }
        }
      }
    }
  }
  return;
}

int main(int argc, char const *argv[]) {
  ll _1[MAX][MAX], _2[MAX][MAX], result[MAX][MAX];
  srand(time(NULL));
  for (int i = 0; i < MAX; i++) {
    for (int j = 0; j < MAX; j++) {
      _1[i][j]=rand()*pow(-1,rand());
      _2[i][j]=rand()*pow(-1,rand());
    }
  }
  clock_t t;
  t=clock();
  int N=MAX;
  multiply(_1,_2,result, MAX);
  t = clock() - t;
  printf ("It took me %d for normal matrix multiplication (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
  for (int i = 0; i < MAX; i++) {
    for (int j = 0; j < MAX; j++) {
      result[i][j]=0;
    }
  }
  t=clock();
  sixloop(_1,_2,result,5, MAX);
  t = clock() - t;
  printf ("It took me %d for 6 matrix multiplication (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);

  return 0;
}

#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>

#define MAX 100
#define ll long long
using namespace std;

int main(int argc, char const *argv[]) {
  ll x[MAX]{2};
  ll temp[MAX]{0};
  clock_t t;
  srand(time(NULL));
  vector<vector<ll> > mtrx;
  vector<ll> owo(MAX,1);
  for(int i=0; i<MAX; ++i){
    mtrx.push_back(owo);
  }
  t=clock();
  for(int i=0; i<MAX; ++i){
    for(int j=0; j<MAX; ++j){
      temp[i]+=mtrx[i][j]*x[j];
    }
  }
  t = clock() - t;
  printf ("It took me %d for 1st (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);


  t=clock();
  for(int j=0; j<MAX; ++j){
    for(int i=0; i<MAX; ++i){
      temp[i]+=mtrx[i][j]*x[j];
    }
  }
  t = clock() - t;
  printf ("It took me %d for 2nd (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
  return 0;
}

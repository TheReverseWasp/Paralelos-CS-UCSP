#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cuda.h>
#include <fstream>

#define uint unsigned int
#define Leaves 256
#define defsize 2
#define arrsize 64 /// if dataset grows increse 64
#define levels 5

using namespace std;


/****************************************add to struct*****************************************************/

__device__
void add_itself (pair <char*, int> **c_dis, int **actual_pos, pair <char*, int> elem, uint lvl) {
  uint tpos = blockIdx.x * blockDim.x + threadIdx.x;
  uint eval = (lvl + 1) * defsize;
  uint peval = lvl * defsize;
  if (actual_pos[tpos][lvl] == eval) {
    if (lvl == levels - 1) {
      return;
    }
    for (uint i = 0; i < eval; i++) {
      (add_itself (c_dis, actual_pos, c_dis[tpos][eval - peval + i], lvl + 1));
    }
    actual_pos[tpos][lvl] = 0;
    (add_itself (c_dis, actual_pos, elem, lvl));
  }
  return;
}

__global__
void add (pair <char*, int> **c_dis, int **actual_pos, pair <char*, int> *lines, uint sizelines) {
  uint tpos = blockIdx.x * blockDim.x + threadIdx.x;
  if (tpos < Leaves) {
    for (uint i = tpos; i < sizelines; i += Leaves) {
      add_itself (c_dis, actual_pos, lines[i], 0);
    }
  }
}
/****************************************search on struct*****************************************************/

__global__
void search (pair <char*, int> **c_dis, int **actual_pos, char* to_search, uint &threadID, uint &elemeID) {
  uint tpos = blockIdx.x * blockDim.x + threadIdx.x;
  if (tpos < Leaves) {
    for (uint j = 0; j < levels; j++) {
      for (uint k = 0; k < actual_pos[tpos][j]; k++) {
        if (to_search == c_dis[tpos][k].first) {
          threadID = tpos;
          elemeID = c_dis[tpos][k].second;
          return;
        }
      }
    }
  }
}





void tempdoll () {
  /////////////////////////////struct///////////////////////////////////////
  pair <char*, int> **c_dis = new pair <char*, int> *[Leaves];
  for (uint i = 0; i < Leaves; ++i) {
    c_dis[i] = new pair <char*, int> [arrsize];
  }
  int **actual_pos = new int* [Leaves];
  for (uint i = 0; i < Leaves; i++) {
    actual_pos[i] = new int [levels];
    for (uint j = 0; j < levels; j++) {
      actual_pos[i][j] = 0;
    }
  }
  int sizepair = Leaves * arrsize * sizeof (pair <char*, int>), sizepos = Leaves * levels * sizeof (int);
  cudaMalloc((void**) c_dis, sizepair);
  cudaMalloc((void**) actual_pos, sizepos);
  /////////////////////////////input///////////////////////////////////////
  vector <pair <char*, int> > linesonv;
  pair <char*, int> temp;
  temp.first = new char[6];
  string templine;
  int c = 0;
  ifstream my_file("dataset.txt");
  while (getline (my_file, templine)) {
    temp.second = c;
    strcpy(temp.first, templine.c_str());
    linesonv.push_back (temp);
    ++c;
  }
  my_file.close ();
  pair <char*, int> *lines = new pair <char*, int> [linesonv.size ()];
  for (uint i = 0; i < linesonv.size (); i++) {
    lines[i] = linesonv[i];
  }
  int sizeinput = linesonv.size() * sizeof(pair <char*, int>);
  cudaMalloc((void **) &lines, sizeinput);
  /////////////////////////////add elements///////////////////////////////////////
  dim3 dimGrid(256);
  dim3 dimBlock(1);       ///modificar en caso de aumentar los threads a usar
  add <<< dimGrid, dimBlock >>> (c_dis, actual_pos, lines, c);
  /////////////////////////////search elements///////////////////////////////////////
  uint threadID, elemeID;
  char *to_search = "jukes";
  search <<< dimGrid, dimBlock >>> (c_dis, actual_pos, to_search, threadID, elemeID);
  cout << to_search << " found on thread " << threadID << " and its value is " << elemeID <<endl;
  ///////////////////////////////////////free///////////////////////////////////////
  cudaFree (lines); cudaFree (c_dis); cudaFree(actual_pos);


}



/**/
int main(int argc, char const *argv[]) {
  tempdoll();
  return 0;
}

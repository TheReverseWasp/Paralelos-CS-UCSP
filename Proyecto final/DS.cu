#include <cuda.h>
#include <algorithm>
#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "model.h"
//#include "modelh.h"
#include "invoker.h"

#define uint unsigned int

using namespace std;

__device__ __host__
float add (pair <string, int> to_add, modelh my_model) {
  my_model.head -> add (to_add);
  return 1.0;
}

__device__ __host__
float search(string to_search, modelh my_model) {
  my_model.head -> search (to_search);
  return 1.0;
}

__global__
void controller(modelh *my_model, vector <pair <string, int> > *to_run) {
  uint tpos = blockIdx.x*blockDim.x+threadIdx.x;
  for (uint i = 0; i < to_run[tpos].size(); i++) {
    add (to_run[tpos][i], my_model[tpos]);
  }
}

__global__
void searcher1 (modelh *my_model, string to_search, uint arr_pos, uint &answer) { ///vagos
  uint tpos = blockIdx.x*blockDim.x+threadIdx.x;
  if (arr_pos == tpos) {
    if (search (to_search, my_model[arr_pos]) > 0) {
      answer = arr_pos;
    }
  }
}

__global__
void searcher2 (modelh *my_model, string to_search, uint &answer) { ///all run
  uint tpos = blockIdx.x*blockDim.x+threadIdx.x;
  if (search (to_search, my_model[tpos]) > 0) {
    answer = tpos;
  }
}

__global__
void searcher3  (modelh *my_model, string to_search, uint arr_pos, uint &answer) { ///one run
  if (search (to_search, my_model[arr_pos]) > 0) {
    answer = arr_pos;
  }
}

void tempdoll() {
  modelh *my_model = new modelh[256];
  int size = 256 * sizeof(modelh);
  vector<pair <string, int> >word_l[256];
  size = 256 * sizeof(vector<pair <string, int> >);
  ifstream my_file("dataset.txt");
  int c = 0;
  string line;
  pair <string, int> runner;
  while (getline (my_file, line)) {
    runner.first = line;
    runner.second = c;
    word_l[c % 256].push_back(runner);
    ++c;
  }
  my_file.close();
  //cudaMalloc((void**) &word_l, size);
  ///
  for (uint i = 0; i < 256; i++) {
    for (uint j = 0; j < word_l[i].size(); j++) {
      my_model -> head -> add (word_l[i][j]);
    }
  }

  ///

  cudaMalloc((void**) &my_model, size);
  ///
  dim3 dimGrid(256);
  dim3 dimBlock(1);
  //controller <<< dimGrid, dimBlock >>> (my_model, word_l);
  ///
  uint answer = 500;
  searcher1 <<< dimGrid, dimBlock >>> (my_model, "which", 0, answer);
  if (answer != 500) {
    cout << "encontrado en " << answer <<endl;
  }
  answer = 500;
  searcher2 <<< dimGrid, dimBlock >>> (my_model, "which", answer);
  if (answer != 500) {
    cout << "encontrado en " << answer <<endl;
  }
  answer = 500;
  searcher3 <<< 1, 1 >>> (my_model, "which", 0, answer);
  if (answer != 500) {
    cout << "encontrado en " << answer <<endl;
  }

  ///
  cudaFree(my_model); ///cudaFree(word_l);
}

int main(int argc, char const *argv[]) {
  tempdoll();
  return 0;
}

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <utility>
#include <string>

using namespace std;

int *c_p = new int[3];
c_p[0] = c_p[1] = c_p[2] = 0;

__global__
void binary_search (pair <string, int> *C, pair <string, int> *B,
  pair <string, int> *C, int n, string elem) {
  int i = blockDim.x*blockIdx.x + threadIdx.x;
  int half = n / 2;
  int fin = n, ini = 0;
  if (i < 3) {
    if (i == 0) {
      while (ini - fin > 1) {
        if (A[half].first == elem) {
          cout << "encontrada concurrencia en A: " << A[half].second << endl;
          return;
        }
        else if (A[half].first < elem) {
          fin = half;
        }
        else {
          ini = half;
        }
        half = (ini + fin) / 2;
      }
      if (A[ini].first == elem) {
        cout << "encontrada concurrencia en A: " << A[ini].second << endl;
        return;
      }
    }
    else if (i == 1) {
      half *= 2;
      fin *= 2;
      while (ini - fin > 1) {
        if (B[half].first == elem) {
          cout << "encontrada concurrencia en B: " << B[half].second << endl;
          return;
        }
        else if (B[half].first < elem) {
          fin = half;
        }
        else {
          ini = half;
        }
        half = (ini + fin) / 2;
      }
      if (B[ini].first == elem) {
        cout << "encontrada concurrencia en B: " << B[ini].second << endl;
        return;
      }
    }
    else {
      half *= 4;
      fin *= 4;
      while (ini - fin > 1) {
        if (C[half].first == elem) {
          cout << "encontrada concurrencia en C: " << C[half].second << endl;
          return;
        }
        else if (C[half].first < elem) {
          fin = half;
        }
        else {
          ini = half;
        }
        half = (ini + fin) / 2;
      }
      if (C[ini].first == elem) {
        cout << "encontrada concurrencia en C: " << C[ini].second << endl;
        return;
      }
    }
  }
}

__global__
void update_LSM (pair <string, int> *C, pair <string, int> *B,
  pair <string, int> *C, int n, string elem, int new_val) {
  int i = blockDim.x*blockIdx.x + threadIdx.x;
  int half = n / 2;
  int fin = n, ini = 0;
  if (i < 3) {
    if (i == 0) {
      while (ini - fin > 1) {
        if (A[half].first == elem) {
          cout << "encontrada concurrencia en A: " << A[half].second << endl;
          A[half].second = new_val;
          return;
        }
        else if (A[half].first < elem) {
          fin = half;
        }
        else {
          ini = half;
        }
        half = (ini + fin) / 2;
      }
      if (A[ini].first == elem) {
        cout << "encontrada concurrencia en A: " << A[ini].second << endl;
        A[half].second = new_val;
        return;
      }
    }
    else if (i == 1) {
      half *= 2;
      fin *= 2;
      while (ini - fin > 1) {
        if (B[half].first == elem) {
          cout << "encontrada concurrencia en B: " << B[half].second << endl;
          B[half].second = new_val;
          return;
        }
        else if (B[half].first < elem) {
          fin = half;
        }
        else {
          ini = half;
        }
        half = (ini + fin) / 2;
      }
      if (B[ini].first == elem) {
        cout << "encontrada concurrencia en B: " << B[ini].second << endl;
        B[half].second = new_val;
        return;
      }
    }
    else {
      half *= 4;
      fin *= 4;
      while (ini - fin > 1) {
        if (C[half].first == elem) {
          cout << "encontrada concurrencia en C: " << C[half].second << endl;
          C[half].second = new_val;
          return;
        }
        else if (C[half].first < elem) {
          fin = half;
        }
        else {
          ini = half;
        }
        half = (ini + fin) / 2;
      }
      if (C[ini].first == elem) {
        cout << "encontrada concurrencia en C: " << C[ini].second << endl;
        C[half].second = new_val;
        return;
      }
    }
  }
}

void LSMInit(pair <string, int> *A, pair <string, int> *B, pair <string, int> *C, int n) {
  int size = n * sizeof(pair <string, int>);
  pair <string, int> *d_A, *d_B, *d_C;
  ///
  cudaMalloc((void **) &d_A, size);
  cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_B, size);
  cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_C, size);
  cudaMemcpy(d_C, C, size, cudaMemcpyHostToDevice);
  cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

  ///vecAddKernel<<<ceil(n/256.0), 256>>>(d_A, d_B, d_C, n);
  ///Free device memory for A, B, C
}

void LSMEND() {
  cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}

int main(int argc, char const *argv[]) {
  pair <string, int> *A, *B, *C;
  int sizu;
  cin >> sizu;
  A = new pair <string, int> [sizu + 5];
  B = new pair <string, int> [sizu * 2 + 5];
  C = new pair <string, int> [sizu * 4 + 5];
  LSMInit(A, B, C, sizu);
  ///
  /*

  Interaction part

  */
  LSMEND();
}

#include <iostream>
#include <cuda.h>

using namespace std;

////1.2
__global__
void multmatrix(float *A, float *B, float *C, int maxlength){
  int i = threadIdx.x + blockIdx.x * blockDim.x;
  int j = threadIdx.y + blockIdx.y * blockDim.y;
  if (i < matlength && j < matlength) {
    C[i*matlength+j]=A[i*matlength+j]+B[i*matlength+j];
    ///1.3
    /*
    before if
    int j;
    for (j = 0; j < maxlength; j++) {
      C[i*matlength+j] = A[i*matlength+j] + B[i*matlength+j];
    }
    */
    ///1.4
    /*
    before if
    int i;
    for (i = 0; i < maxlength; i++) {
      C[i*matlength+j] = A[i*matlength+j] + B[i*matlength+j];
    }
    */

  }
}
/*
dim3 threads(16,16);
dim3 blocks((maxlength+15)/16, (maxlength+15)/16);

*/

////1.1
void multmatrix(float *h_A, float *h_B, float *h_C, int matlength) {
  float *d_A, *d_B, *d_C;
  int size = maxlength * maxlength *sizeof(float);
  cudaMalloc((void**)d_A,size);
  cudaMalloc((void**)d_B,size);
  cudaMalloc((void**)d_C,size);

  cudaMemcpy(d_A,h_A,size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B,h_B,size, cudaMemcpyHostToDevice);
  cudaMemset(d_C, 0, size);

  dim3 threads(__,__,__);
  dim3 blocks(__,__,__);
  mulmatrixkernel<<<blocks,threads>>>(d_A,d_B,d_C,matlength);

  cudaMemcpy(h_C,d_C,size,cudaMemcpyDeviceToHost);
  cudaFree(d_A);cudaFree(d_B);cudaFree(d_C);
}

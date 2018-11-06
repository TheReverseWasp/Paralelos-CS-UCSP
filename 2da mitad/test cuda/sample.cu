#include <stdio.h>

__global__
void vecAddKernel(float* A, float* B, float* C, int n)
{
  int i = blockDim.x*blockIdx.x + threadIdx.x;
  if(i < n) C[i] = A[i] + B[i];
}

void vecAdd(float* A, float* B, float* C, int n) {
  int size = n * sizeof(float);
  float *d_A, *d_B, *d_C;
  ///
  cudaMalloc((void **) &d_A, size);
  cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_B, size);
  cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

  cudaMalloc((void **) &d_C, size);

  vecAddKernel<<<ceil(n/256.0), 256>>>(d_A, d_B, d_C, n);

  cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
  ///Free device memory for A, B, C
  cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}
int main(int argc, char const *argv[]) {
  float *A, *B, *C;
  int n = 10000;
  A = new float[n];
  B = new float[n];
  C = new float[n];
  for (int i = 0; i < n; i++) {
    A[i] = i / 100.0;
    B[i] = i;
    C[i] = 0;
  }
  vecAdd (A, B, C, n);
  for (int i = 0; i < n; i++) {
    printf("%f\n", C[i]);
  }
  return 0;
}

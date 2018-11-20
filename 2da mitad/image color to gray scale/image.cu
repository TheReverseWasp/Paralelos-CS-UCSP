#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;

int BLUR_SIZE = 5;
int height = 800;
int width = 800;

__global__
void blurKerner(unsigned char * in, unsigned char * out,
int w, int h)
{
  int Col = blockIdx.x * blockDim.x + threadIdx.x;
  int Row = blockIdx.y * blockDim.y + threadIdx.y;
  ////
  if (Col < w && Row < h) {
    int pixVal = 0;
    int pixels = 0;
    for (int blurRow = -BLUR_SIZE; blurRow < BLUR_SIZE + 1
      ; blurRow++) {
      for (int blurCol = -BLUR_SIZE; blurCol < BLUR_SIZE + 1;
         blurCol++) {
        int curRow = Row + blurRow;
        int curCol = Col + blurCol;
        if (curRow > -1 && curRow < h && curCol > -1 &&
        curCol < w) {
          pixVal +=in[curRow * w + curCol];
          pixels++;
        }
      }
    }
    out[Row * w + Col] = unsigned char(pixVal / pixels);
  }
}
////
void tempdoll(unsigned char *buffer,
  unsigned char *blured_ret, int w, int h){
  int size = w * h * sizeof(unsigned char);
  unsigned char blured[w * h], grayed[w * h],
  D_b[w * h];
  ///
  cudaMalloc((void**) &D_b, size);
  cudaMemcpy(D_b, buffer, cudaMemcpyHostToDevice);
  cudaMalloc((void**) &blured, size);
  ///
  blurKerner <<< ceil((w * h)/256.0), 256 >>> (D_b, blured, w, h);
  cudaMemcpy(blured_ret, blured, cudaMemcpyDeviceToHost);
  ///
  cudaFree(D_b); cudaFree(blured); //cudaFree(D_b);
}

//// convertir a Mat un arreglo
Mat convertToMat(unsigned char *buffer) {
  Mat tmp(width, height, CV_8UC1);
  for (int x = 0; x < height; x++) {
    for (int y = 0; y < width; y++) {
      int value = (int) buffer[x * width + y];
      tmp.at<int>(y, x) = value;
    }
  }
  return tmp;
}







int main(int argc, char const *argv[]) {
  unsigned char *Pout
  char* imageName = argv[1];
  Mat image;
  image = imread( imageName, 1 );
  ///

  if( argc != 2 || !image.data )
  {
   printf( " No image data \n " );
   return -1;
  }
  ///

  unsigned char buffer[height * width];
  unsigned char out_blur[height * width];
  unsigned char out_gray[height * width];
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      uchar& uxy = image.at<uchar>(j, i);
      unsigned char color = (unsigned char) uxy;
      buffer[j * width + i] = color;
    }
  }
  ///
  tempdoll(buffer, out_blur, width, height);

  Mat blured_img = convertToMat(out_blur);
  imwrite( "blured_Image.jpg", blured_img );
  return 0;
}

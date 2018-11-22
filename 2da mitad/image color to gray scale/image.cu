#include <stdio.h>
#include <cuda.h>
#include <vector>

#include "lodepng.h"
#include "helperfuns.h"
using namespace std;

__global__
void blurKerner(unsigned char * in, unsigned char * out,
int w, int h)
{
  int BLUR_SIZE = 5;
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
        if (curRow > -1 && curRow < h  && curCol > -1 &&
        curCol < w) {
          pixVal +=in[curRow * w + curCol];
          pixels++;
        }
      }
    }
    out[Row * w + Col] = (unsigned char)(pixVal / pixels);
  }
}
////grayscale function
__global__
void colorToGreyscaleConversion(unsigned char * Pout, unsigned
  char * Pin, int width, int height) {
  int Col = threadIdx.x + blockIdx.x * blockDim.x;
  int Row = threadIdx.y + blockIdx.y * blockDim.y;
  if (Col < width && Row < height) {
  // get 1D coordinate for the grayscale image
    int greyOffset = Row*width + Col;
    // one can think of the RGB image having
    // CHANNEL times columns than the grayscale image
    int rgbOffset = greyOffset * 3;
    unsigned char r = Pin[rgbOffset  ]; // red value for pixel
    unsigned char g = Pin[rgbOffset + 2]; // green value for pixel
    unsigned char b = Pin[rgbOffset + 3]; // blue value for pixel
    // perform the rescaling and store it
    // We multiply by floating point constants
    Pout[greyOffset] = 0.21f*r + 0.07f*b + 0.71f*g;
  }
}

////temper function
void tempdoll(unsigned char *buffer,
  unsigned char *blured_ret, unsigned char *grayed_ret, int w, int h){
  int size = w * h * sizeof(unsigned char);
  unsigned char *blured = new unsigned char[w * h],
  *grayed = new unsigned char[w * h], *D_b = new unsigned char[w * h];
  ///
  cudaMalloc((void**) &D_b, size);
  cudaMemcpy(D_b, buffer, size, cudaMemcpyHostToDevice);
  cudaMalloc((void**) &blured, size);
  cudaMalloc((void**) &grayed, size);
  ///
  dim3 dimBlock(256, 1, 1);
  dim3 dimBlock(16, 16, 1);
  blurKerner <<< ceil((w * h)/256.0), dimBlock >>> (D_b, blured, w, h);
  colorToGreyscaleConversion <<<
   ceil((w * h) / 256.0), dimBlock >>>
   (grayed, D_b, w, h);
  cudaMemcpy(blured_ret, blured, size, cudaMemcpyDeviceToHost);
  cudaMemcpy(grayed_ret, grayed, size, cudaMemcpyDeviceToHost);
  ///
  cudaFree(D_b); cudaFree(blured); cudaFree(grayed);
}

int main(int argc, char const *argv[]) {
  vector <unsigned char> buffer;
  unsigned w1,h1;
  const char* filename = "cat.png";
  //decode
  unsigned error = lodepng::decode(buffer, w1, h1, filename);
  int w = w1, h = h1;
  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
  vector <unsigned char> blured(buffer.size()), gray(buffer.size());
  unsigned char *buffer1 = new unsigned char[w*h*4], *blured1 = new unsigned char[w*h*4],
  *gray1 = new unsigned char[w*h*4];
  vectoarr(buffer, buffer1);
  ///
  tempdoll(buffer1, blured1, gray1, w, h);
  ////
  arrtovec(blured1, blured); arrtovec(gray1, gray);
  ///

  vector<unsigned char> png1, png2;
  error = lodepng::encode(png1, blured, w, h);
  if(!error) lodepng::save_file(png1, "catb.png");
  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;


  error = lodepng::encode(png2, gray, w, h);
  if(!error) lodepng::save_file(png2, "catg.png");
  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;




  return 0;
}

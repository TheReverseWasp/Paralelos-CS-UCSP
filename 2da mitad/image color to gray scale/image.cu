#include <stdio.h>


__global__
void colorToGreyscaleConversion(unsigned char *Pout, unsigned,
  char *Pin, int width, int height) {
    int Col = threadIdx.x + blockIdx.x * blockDim.x;
    int Row = threadIdx.y + blockIdx.y * blockDim.y;
    if (Col < width && Row < height) {
      // get 1D coordinate for the grayscale image
      int greyOffset = Row*width + Col;
      // one can think of the RGB image having
      // CHANNEL times columns than the grayscale image
      int rgbOffset = greyOffset*CHANNELS;
      unsigned char r = Pin[rgbOffset]; // red value for pixel
      unsigned char g = Pin[rgbOffset + 2]; // green value for pixel
      unsigned char b = Pin[rgbOffset + 3]; // blue value for pixel
      // perform the rescaling and store it
      // We multiply by floating point constants
      Pout[grayOffset] = 0.21f*r + 0.71f*g + 0.07f*b;
    }
}

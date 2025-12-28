#ifndef IMAGE_IO_H
#define IMAGE_IO_H

// Standard image loader/saver declarations
unsigned char* loadImage(const char* filename, int* width, int* height, int* channels);
void saveImage(const char* filename, int width, int height, int channels, unsigned char* data);
void freeImage(unsigned char* data);

#endif
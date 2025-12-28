#ifndef FILTERS_H
#define FILTERS_H

#include <vector>

// Requirement: All 5 filters must be implemented
void applyGrayscale(unsigned char* data, int width, int height, int channels);
void applyBrightness(unsigned char* data, int width, int height, int channels, int amount);
void applyGaussianBlur(const unsigned char* input, unsigned char* output, int width, int height, int channels);
void applySobel(const unsigned char* input, unsigned char* output, int width, int height, int channels);
void applySharpen(const unsigned char* input, unsigned char* output, int width, int height, int channels);

#endif
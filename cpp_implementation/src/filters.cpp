#include "filters.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// 1. Grayscale Conversion (Luminance Formula)
void applyGrayscale(unsigned char* data, int width, int height, int channels) {
    for (int i = 0; i < width * height; i++) {
        int r = data[i * channels];
        int g = data[i * channels + 1];
        int b = data[i * channels + 2];
        unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
        data[i * channels] = data[i * channels + 1] = data[i * channels + 2] = gray;
    }
}

// 5. Brightness Adjustment
void applyBrightness(unsigned char* data, int width, int height, int channels, int amount) {
    for (int i = 0; i < width * height * channels; i++) {
        int val = data[i] + amount;
        // Manual clamp to ensure it stays between 0-255
        data[i] = (unsigned char)std::max(0, std::min(255, val));
    }
}

// Helper for convolution filters (Blur, Sobel, Sharpen)
// This ensures we handle the image borders and channels correctly
unsigned char getPixel(const unsigned char* data, int x, int y, int c, int width, int height, int channels) {
    if (x < 0 || x >= width || y < 0 || y >= height) return 0;
    return data[(y * width + x) * channels + c];
}

// 2. Gaussian Blur (3x3 Kernel)
void applyGaussianBlur(const unsigned char* input, unsigned char* output, int width, int height, int channels) {
    float kernel[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                float sum = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        sum += getPixel(input, x + kx, y + ky, c, width, height, channels) * kernel[ky + 1][kx + 1];
                    }
                }
                output[(y * width + x) * channels + c] = (unsigned char)sum;
            }
        }
    }
}

// 3. Edge Detection (Sobel Filter)
void applySobel(const unsigned char* input, unsigned char* output, int width, int height, int channels) {
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                float sumX = 0, sumY = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        unsigned char val = getPixel(input, x + kx, y + ky, c, width, height, channels);
                        sumX += val * Gx[ky + 1][kx + 1];
                        sumY += val * Gy[ky + 1][kx + 1];
                    }
                }
                int val = std::sqrt(sumX * sumX + sumY * sumY);
                output[(y * width + x) * channels + c] = (unsigned char)std::min(255, val);
            }
        }
    }
}

// 4. Sharpening
void applySharpen(const unsigned char* input, unsigned char* output, int width, int height, int channels) {
    int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        sum += getPixel(input, x + kx, y + ky, c, width, height, channels) * kernel[ky + 1][kx + 1];
                    }
                }
                output[(y * width + x) * channels + c] = (unsigned char)std::max(0, std::min(255, sum));
            }
        }
    }
}
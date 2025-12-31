//OpenMP implementation
#include <iostream>
#include <vector>
#include <string>
#include <cmath> 
#include <omp.h>
#include <cmath>
#include <algorithm>
#include "filters.h"
#include "image_io.h"
#include "utils.h"

// Parallel Grayscale using OpenMP
void applyGrayscaleOMP(unsigned char* data, int width, int height, int channels) {
    int totalPixels = width * height;
    
    #pragma omp parallel for
    for (int i = 0; i < totalPixels; i++) {
        int r = data[i * channels];
        int g = data[i * channels + 1];
        int b = data[i * channels + 2];
        unsigned char gray = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
        data[i * channels] = data[i * channels + 1] = data[i * channels + 2] = gray;
    }
}

// Parallel Brightness using OpenMP
void applyBrightnessOMP(unsigned char* data, int width, int height, int channels, int amount) {
    int totalElements = width * height * channels;
    
    #pragma omp parallel for
    for (int i = 0; i < totalElements; i++) {
        int val = data[i] + amount;
        data[i] = clamp(val);
    }
}

// Parallel Gaussian Blur using OpenMP
void applyGaussianBlurOMP(const unsigned char* input, unsigned char* output, 
                          int width, int height, int channels) {
    float kernel[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };

    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                float sum = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int px = std::max(0, std::min(width - 1, x + kx));
                        int py = std::max(0, std::min(height - 1, y + ky));
                        sum += input[(py * width + px) * channels + c] * kernel[ky + 1][kx + 1];
                    }
                }
                output[(y * width + x) * channels + c] = static_cast<unsigned char>(sum);
            }
        }
    }
}

// Parallel Sobel using OpenMP
void applySobelOMP(const unsigned char* input, unsigned char* output, 
                   int width, int height, int channels) {
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                float sumX = 0, sumY = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int px = std::max(0, std::min(width - 1, x + kx));
                        int py = std::max(0, std::min(height - 1, y + ky));
                        unsigned char val = input[(py * width + px) * channels + c];
                        sumX += val * Gx[ky + 1][kx + 1];
                        sumY += val * Gy[ky + 1][kx + 1];
                    }
                }
                int val = static_cast<int>(std::sqrt(sumX * sumX + sumY * sumY));
                output[(y * width + x) * channels + c] = clamp(val);
            }
        }
    }
}

// Parallel Sharpen using OpenMP
void applySharpenOMP(const unsigned char* input, unsigned char* output, 
                     int width, int height, int channels) {
    int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int px = std::max(0, std::min(width - 1, x + kx));
                        int py = std::max(0, std::min(height - 1, y + ky));
                        sum += input[(py * width + px) * channels + c] * kernel[ky + 1][kx + 1];
                    }
                }
                output[(y * width + x) * channels + c] = clamp(sum);
            }
        }
    }
}

int main() {
    std::string inputPath = "../../data/input/food_samples/food1.jpg";
    std::string outputDir = "../../data/output/openmp/";

    int width, height, channels;
    
    printProgress("Loading input image...");
    unsigned char* originalData = loadImage(inputPath.c_str(), &width, &height, &channels);

    if (originalData == nullptr) {
        std::cerr << "Error: Could not load " << inputPath << std::endl;
        return -1;
    }
    std::cout << "Image Size: " << width << "x" << height << " (" << channels << " channels)" << std::endl;

    // Set number of OpenMP threads
    int numThreads = omp_get_max_threads();
    omp_set_num_threads(numThreads);
    std::cout << "Using " << numThreads << " OpenMP threads" << std::endl;

    size_t imageSize = width * height * channels;
    unsigned char* outputBuffer = new unsigned char[imageSize];

    // Filter 1: Grayscale (OpenMP)
    {
        printProgress("Applying Grayscale (OpenMP)...");
        std::vector<unsigned char> grayscaleCopy(originalData, originalData + imageSize);
        Timer timer("Grayscale (OpenMP)");
        applyGrayscaleOMP(grayscaleCopy.data(), width, height, channels);
        saveImage((outputDir + "1_grayscale_openmp.jpg").c_str(), width, height, channels, grayscaleCopy.data());
    }

    // Filter 2: Brightness (OpenMP)
    {
        printProgress("Applying Brightness (OpenMP)...");
        std::vector<unsigned char> brightnessCopy(originalData, originalData + imageSize);
        Timer timer("Brightness (OpenMP)");
        applyBrightnessOMP(brightnessCopy.data(), width, height, channels, 50);
        saveImage((outputDir + "2_brightness_openmp.jpg").c_str(), width, height, channels, brightnessCopy.data());
    }

    // Filter 3: Gaussian Blur (OpenMP)
    {
        printProgress("Applying Gaussian Blur (OpenMP)...");
        Timer timer("Gaussian Blur (OpenMP)");
        applyGaussianBlurOMP(originalData, outputBuffer, width, height, channels);
        saveImage((outputDir + "3_gaussian_blur_openmp.jpg").c_str(), width, height, channels, outputBuffer);
    }

    // Filter 4: Sobel Edge Detection (OpenMP)
    {
        printProgress("Applying Sobel Edges (OpenMP)...");
        Timer timer("Sobel (OpenMP)");
        applySobelOMP(originalData, outputBuffer, width, height, channels);
        saveImage((outputDir + "4_sobel_edges_openmp.jpg").c_str(), width, height, channels, outputBuffer);
    }

    // Filter 5: Sharpen (OpenMP)
    {
        printProgress("Applying Sharpening (OpenMP)...");
        Timer timer("Sharpen (OpenMP)");
        applySharpenOMP(originalData, outputBuffer, width, height, channels);
        saveImage((outputDir + "5_sharpened_openmp.jpg").c_str(), width, height, channels, outputBuffer);
    }

    std::cout << "\n=== All OpenMP filters processed successfully! ===" << std::endl;
    std::cout << "Check the 'data/output/openmp/' folder for results." << std::endl;

    freeImage(originalData);
    delete[] outputBuffer;

    return 0;
}
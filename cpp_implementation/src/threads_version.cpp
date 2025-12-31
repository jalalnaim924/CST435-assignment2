//std::thread implementation
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <algorithm>
#include "filters.h"
#include "image_io.h"
#include "utils.h"

// Parallel Grayscale using std::thread
void applyGrayscaleThreaded(unsigned char* data, int width, int height, int channels, int numThreads) {
    auto worker = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            int r = data[i * channels];
            int g = data[i * channels + 1];
            int b = data[i * channels + 2];
            unsigned char gray = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
            data[i * channels] = data[i * channels + 1] = data[i * channels + 2] = gray;
        }
    };

    int totalPixels = width * height;
    int pixelsPerThread = totalPixels / numThreads;
    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; t++) {
        int start = t * pixelsPerThread;
        int end = (t == numThreads - 1) ? totalPixels : (t + 1) * pixelsPerThread;
        threads.emplace_back(worker, start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

// Parallel Brightness using std::thread
void applyBrightnessThreaded(unsigned char* data, int width, int height, int channels, 
                              int amount, int numThreads) {
    auto worker = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            int val = data[i] + amount;
            data[i] = clamp(val);
        }
    };

    int totalElements = width * height * channels;
    int elementsPerThread = totalElements / numThreads;
    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; t++) {
        int start = t * elementsPerThread;
        int end = (t == numThreads - 1) ? totalElements : (t + 1) * elementsPerThread;
        threads.emplace_back(worker, start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

// Parallel Gaussian Blur using std::thread
void applyGaussianBlurThreaded(const unsigned char* input, unsigned char* output, 
                                int width, int height, int channels, int numThreads) {
    float kernel[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };

    auto worker = [&](int startY, int endY) {
        for (int y = startY; y < endY; y++) {
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
    };

    int rowsPerThread = height / numThreads;
    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; t++) {
        int startY = t * rowsPerThread;
        int endY = (t == numThreads - 1) ? height : (t + 1) * rowsPerThread;
        threads.emplace_back(worker, startY, endY);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

// Parallel Sobel using std::thread
void applySobelThreaded(const unsigned char* input, unsigned char* output, 
                        int width, int height, int channels, int numThreads) {
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    auto worker = [&](int startY, int endY) {
        for (int y = startY; y < endY; y++) {
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
    };

    int rowsPerThread = height / numThreads;
    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; t++) {
        int startY = t * rowsPerThread;
        int endY = (t == numThreads - 1) ? height : (t + 1) * rowsPerThread;
        threads.emplace_back(worker, startY, endY);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

// Parallel Sharpen using std::thread
void applySharpenThreaded(const unsigned char* input, unsigned char* output, 
                          int width, int height, int channels, int numThreads) {
    int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    auto worker = [&](int startY, int endY) {
        for (int y = startY; y < endY; y++) {
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
    };

    int rowsPerThread = height / numThreads;
    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; t++) {
        int startY = t * rowsPerThread;
        int endY = (t == numThreads - 1) ? height : (t + 1) * rowsPerThread;
        threads.emplace_back(worker, startY, endY);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    std::string inputPath = "../../data/input/food_samples/food1.jpg";
    std::string outputDir = "../../data/output/threads/";

    int width, height, channels;
    
    printProgress("Loading input image...");
    unsigned char* originalData = loadImage(inputPath.c_str(), &width, &height, &channels);

    if (originalData == nullptr) {
        std::cerr << "Error: Could not load " << inputPath << std::endl;
        return -1;
    }
    std::cout << "Image Size: " << width << "x" << height << " (" << channels << " channels)" << std::endl;

    // Determine optimal number of threads
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    std::cout << "Using " << numThreads << " threads" << std::endl;

    size_t imageSize = width * height * channels;
    unsigned char* outputBuffer = new unsigned char[imageSize];

    // Filter 1: Grayscale (Parallel)
    {
        printProgress("Applying Grayscale (Threaded)...");
        std::vector<unsigned char> grayscaleCopy(originalData, originalData + imageSize);
        Timer timer("Grayscale (Threaded)");
        applyGrayscaleThreaded(grayscaleCopy.data(), width, height, channels, numThreads);
        saveImage((outputDir + "1_grayscale_threaded.jpg").c_str(), width, height, channels, grayscaleCopy.data());
    }

    // Filter 2: Brightness (Parallel)
    {
        printProgress("Applying Brightness (Threaded)...");
        std::vector<unsigned char> brightnessCopy(originalData, originalData + imageSize);
        Timer timer("Brightness (Threaded)");
        applyBrightnessThreaded(brightnessCopy.data(), width, height, channels, 50, numThreads);
        saveImage((outputDir + "2_brightness_threaded.jpg").c_str(), width, height, channels, brightnessCopy.data());
    }

    // Filter 3: Gaussian Blur (Parallel)
    {
        printProgress("Applying Gaussian Blur (Threaded)...");
        Timer timer("Gaussian Blur (Threaded)");
        applyGaussianBlurThreaded(originalData, outputBuffer, width, height, channels, numThreads);
        saveImage((outputDir + "3_gaussian_blur_threaded.jpg").c_str(), width, height, channels, outputBuffer);
    }

    // Filter 4: Sobel Edge Detection (Parallel)
    {
        printProgress("Applying Sobel Edges (Threaded)...");
        Timer timer("Sobel (Threaded)");
        applySobelThreaded(originalData, outputBuffer, width, height, channels, numThreads);
        saveImage((outputDir + "4_sobel_edges_threaded.jpg").c_str(), width, height, channels, outputBuffer);
    }

    // Filter 5: Sharpen (Parallel)
    {
        printProgress("Applying Sharpening (Threaded)...");
        Timer timer("Sharpen (Threaded)");
        applySharpenThreaded(originalData, outputBuffer, width, height, channels, numThreads);
        saveImage((outputDir + "5_sharpened_threaded.jpg").c_str(), width, height, channels, outputBuffer);
    }

    std::cout << "\n=== All threaded filters processed successfully! ===" << std::endl;
    std::cout << "Check the 'data/output/threads/' folder for results." << std::endl;

    freeImage(originalData);
    delete[] outputBuffer;

    return 0;
}
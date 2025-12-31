//std::thread implementation of image processing filters
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <cmath>
#include <algorithm>
#include <filesystem>
#include "filters.h"
#include "image_io.h"
#include "utils.h"

namespace fs = std::filesystem;

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

void processWithThreadCount(const std::vector<std::string>& imageFiles, 
                            const std::string& inputDir, 
                            const std::string& baseOutputDir, 
                            int numThreads) {
    std::string outputDir = baseOutputDir + std::to_string(numThreads) + "_threads/";
    fs::create_directories(outputDir);
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Processing with " << numThreads << " thread(s) (std::thread)" << std::endl;
    std::cout << "========================================\n" << std::endl;

    Timer totalTimer("Total time for " + std::to_string(numThreads) + " threads");

    for (size_t imgIdx = 0; imgIdx < imageFiles.size(); imgIdx++) {
        std::string inputPath = imageFiles[imgIdx];
        std::string filename = fs::path(inputPath).filename().string();
        std::string baseName = fs::path(inputPath).stem().string();
        
        std::cout << "[" << (imgIdx + 1) << "/" << imageFiles.size() << "] " << filename << std::endl;

        int width, height, channels;
        unsigned char* originalData = loadImage(inputPath.c_str(), &width, &height, &channels);

        if (originalData == nullptr) {
            std::cerr << "  Error loading, skipping..." << std::endl;
            continue;
        }

        size_t imageSize = width * height * channels;
        unsigned char* outputBuffer = new unsigned char[imageSize];

        // Apply all 5 filters
        std::vector<unsigned char> grayscaleCopy(originalData, originalData + imageSize);
        applyGrayscaleThreaded(grayscaleCopy.data(), width, height, channels, numThreads);
        saveImage((outputDir + baseName + "_grayscale.jpg").c_str(), 
                 width, height, channels, grayscaleCopy.data());

        std::vector<unsigned char> brightnessCopy(originalData, originalData + imageSize);
        applyBrightnessThreaded(brightnessCopy.data(), width, height, channels, 50, numThreads);
        saveImage((outputDir + baseName + "_brightness.jpg").c_str(), 
                 width, height, channels, brightnessCopy.data());

        applyGaussianBlurThreaded(originalData, outputBuffer, width, height, channels, numThreads);
        saveImage((outputDir + baseName + "_blur.jpg").c_str(), 
                 width, height, channels, outputBuffer);

        applySobelThreaded(originalData, outputBuffer, width, height, channels, numThreads);
        saveImage((outputDir + baseName + "_sobel.jpg").c_str(), 
                 width, height, channels, outputBuffer);

        applySharpenThreaded(originalData, outputBuffer, width, height, channels, numThreads);
        saveImage((outputDir + baseName + "_sharpen.jpg").c_str(), 
                 width, height, channels, outputBuffer);

        freeImage(originalData);
        delete[] outputBuffer;
    }
}

int main() {
    std::string inputDir = "../../data/input/food_samples/";
    std::string baseOutputDir = "../../data/output/cpp_threads/";

    // Get all JPG files
    std::vector<std::string> imageFiles;
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        std::string ext = entry.path().extension().string();
        if (ext == ".jpg" || ext == ".JPG" || ext == ".jpeg" || ext == ".JPEG") {
            imageFiles.push_back(entry.path().string());
        }
    }

    if (imageFiles.empty()) {
        std::cerr << "No JPG images found in " << inputDir << std::endl;
        return -1;
    }

    std::cout << "Found " << imageFiles.size() << " images to process" << std::endl;

    // Process with different thread counts: 1, 2, 4, 8
    std::vector<int> threadCounts = {1, 2, 4, 8};
    
    for (int numThreads : threadCounts) {
        processWithThreadCount(imageFiles, inputDir, baseOutputDir, numThreads);
    }

    std::cout << "\n=== All std::thread processing complete! ===" << std::endl;
    std::cout << "Processed " << imageFiles.size() << " images with 1, 2, 4, and 8 threads" << std::endl;
    std::cout << "Output directory: " << baseOutputDir << std::endl;

    return 0;
}
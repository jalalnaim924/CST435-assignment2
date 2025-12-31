#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "filters.h"
#include "image_io.h"
#include "utils.h"   // <-- IMPORTANT: Timer

namespace fs = std::filesystem;

int main() {
    // ===============================
    // Configuration
    // ===============================
    std::string inputDir  = "../data/input/food_samples/";
    std::string outputDir = "../data/output/sequential/";

    // Create output directory if it doesn't exist
    fs::create_directories(outputDir);

    // ===============================
    // Collect input images
    // ===============================
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

    std::cout << "Found " << imageFiles.size() << " images to process\n" << std::endl;

    // ===============================
    // START TOTAL TIMER (Sequential)
    // ===============================
    Timer totalTimer("Total sequential execution time");

    // ===============================
    // Process each image SEQUENTIALLY
    // ===============================
    for (size_t imgIdx = 0; imgIdx < imageFiles.size(); imgIdx++) {
        std::string inputPath = imageFiles[imgIdx];
        std::string filename = fs::path(inputPath).filename().string();
        std::string baseName = fs::path(inputPath).stem().string();

        std::cout << "Processing [" << (imgIdx + 1) << "/" << imageFiles.size()
                  << "]: " << filename << std::endl;

        int width, height, channels;
        unsigned char* originalData =
            loadImage(inputPath.c_str(), &width, &height, &channels);

        if (originalData == nullptr) {
            std::cerr << "  Error loading image, skipping..." << std::endl;
            continue;
        }

        size_t imageSize = width * height * channels;
        unsigned char* outputBuffer = new unsigned char[imageSize];

        // -------------------------------
        // Filter 1: Grayscale
        // -------------------------------
        {
            std::vector<unsigned char> grayscaleCopy(
                originalData, originalData + imageSize);

            applyGrayscale(grayscaleCopy.data(), width, height, channels);

            saveImage((outputDir + baseName + "_grayscale.jpg").c_str(),
                      width, height, channels, grayscaleCopy.data());
        }

        // -------------------------------
        // Filter 2: Brightness
        // -------------------------------
        {
            std::vector<unsigned char> brightnessCopy(
                originalData, originalData + imageSize);

            applyBrightness(brightnessCopy.data(), width, height, channels, 50);

            saveImage((outputDir + baseName + "_brightness.jpg").c_str(),
                      width, height, channels, brightnessCopy.data());
        }

        // -------------------------------
        // Filter 3: Gaussian Blur
        // -------------------------------
        applyGaussianBlur(originalData, outputBuffer, width, height, channels);
        saveImage((outputDir + baseName + "_blur.jpg").c_str(),
                  width, height, channels, outputBuffer);

        // -------------------------------
        // Filter 4: Sobel Edge Detection
        // -------------------------------
        applySobel(originalData, outputBuffer, width, height, channels);
        saveImage((outputDir + baseName + "_sobel.jpg").c_str(),
                  width, height, channels, outputBuffer);

        // -------------------------------
        // Filter 5: Sharpen
        // -------------------------------
        applySharpen(originalData, outputBuffer, width, height, channels);
        saveImage((outputDir + baseName + "_sharpen.jpg").c_str(),
                  width, height, channels, outputBuffer);

        // Cleanup
        freeImage(originalData);
        delete[] outputBuffer;

        std::cout << "  Completed: " << baseName << std::endl;
    }

    // ===============================
    // Timer prints automatically here
    // ===============================

    std::cout << "\n=== Sequential processing complete! ===" << std::endl;
    std::cout << "Processed " << imageFiles.size() << " images" << std::endl;
    std::cout << "Output directory: " << outputDir << std::endl;

    return 0;
}

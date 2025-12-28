#include <iostream>
#include <vector>
#include <string>
#include "filters.h"
#include "image_io.h"

int main() {
    // 1. Configuration - Paths relative to where you run the executable
    // Note: Make sure food1.jpg exists in this folder or change the path
    std::string inputPath = "../../data/input/food_samples/food1.jpg";
    std::string outputDir = "../../data/output/";

    int width, height, channels;
    
    // 2. Load the source image
    std::cout << "Step 1: Loading input image..." << std::endl;
    unsigned char* originalData = loadImage(inputPath.c_str(), &width, &height, &channels);

    if (originalData == nullptr) {
        std::cerr << "Error: Could not load " << inputPath << ". Make sure the file exists!" << std::endl;
        return -1;
    }
    std::cout << "Success! Image Size: " << width << "x" << height << " (" << channels << " channels)" << std::endl;

    // 3. Create a buffer for filters that need a separate output (Blur, Sobel, Sharpen)
    // This is because these filters read neighbors; if we change pixels mid-way, it ruins the math.
    size_t imageSize = width * height * channels;
    unsigned char* outputBuffer = new unsigned char[imageSize];

    // --- APPLY FILTERS SEQUENTIALLY ---

    // Filter 1: Grayscale
    std::cout << "Step 2: Applying Grayscale..." << std::endl;
    // We work on a copy because Grayscale is "in-place"
    std::vector<unsigned char> grayscaleCopy(originalData, originalData + imageSize);
    applyGrayscale(grayscaleCopy.data(), width, height, channels);
    saveImage((outputDir + "1_grayscale.jpg").c_str(), width, height, channels, grayscaleCopy.data());

    // Filter 2: Brightness
    std::cout << "Step 3: Applying Brightness (+50)..." << std::endl;
    std::vector<unsigned char> brightnessCopy(originalData, originalData + imageSize);
    applyBrightness(brightnessCopy.data(), width, height, channels, 50);
    saveImage((outputDir + "2_brightness.jpg").c_str(), width, height, channels, brightnessCopy.data());

    // Filter 3: Gaussian Blur
    std::cout << "Step 4: Applying Gaussian Blur..." << std::endl;
    applyGaussianBlur(originalData, outputBuffer, width, height, channels);
    saveImage((outputDir + "3_gaussian_blur.jpg").c_str(), width, height, channels, outputBuffer);

    // Filter 4: Sobel Edge Detection
    std::cout << "Step 5: Applying Sobel Edges..." << std::endl;
    applySobel(originalData, outputBuffer, width, height, channels);
    saveImage((outputDir + "4_sobel_edges.jpg").c_str(), width, height, channels, outputBuffer);

    // Filter 5: Sharpen
    std::cout << "Step 6: Applying Sharpening..." << std::endl;
    applySharpen(originalData, outputBuffer, width, height, channels);
    saveImage((outputDir + "5_sharpened.jpg").c_str(), width, height, channels, outputBuffer);

    // 4. Cleanup
    std::cout << "\nAll sequential filters processed successfully!" << std::endl;
    std::cout << "Check the 'data/output/' folder for results." << std::endl;

    freeImage(originalData);
    delete[] outputBuffer;

    return 0;
}
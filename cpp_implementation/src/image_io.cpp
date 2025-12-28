#include "image_io.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
#include <iostream>

unsigned char* loadImage(const char* filename, int* width, int* height, int* channels) {
    unsigned char* data = stbi_load(filename, width, height, channels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load image: " << filename << std::endl;
    }
    return data;
}

void saveImage(const char* filename, int width, int height, int channels, unsigned char* data) {
    if (!stbi_write_jpg(filename, width, height, channels, data, 100)) {
        std::cerr << "Failed to save image: " << filename << std::endl;
    }
}

void freeImage(unsigned char* data) {
    stbi_image_free(data);
}
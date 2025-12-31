//common ultities
#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <iostream>
#include <string>

// Timer class for benchmarking
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::string name;

public:
    Timer(const std::string& timer_name) : name(timer_name) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time
        ).count();
        std::cout << name << " took: " << duration << " ms" << std::endl;
    }

    // Get elapsed time without destroying timer
    double elapsed() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time
        ).count();
    }
};

// Helper to print progress
inline void printProgress(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

// Helper to clamp values between 0 and 255
inline unsigned char clamp(int value) {
    return static_cast<unsigned char>(
        value < 0 ? 0 : (value > 255 ? 255 : value)
    );
}

// Calculate optimal number of threads based on image size
inline int calculateOptimalThreads(int imageSize, int maxThreads) {
    // Use hardware concurrency as base
    const int minPixelsPerThread = 10000;
    int optimalThreads = imageSize / minPixelsPerThread;
    
    if (optimalThreads < 1) optimalThreads = 1;
    if (optimalThreads > maxThreads) optimalThreads = maxThreads;
    
    return optimalThreads;
}

#endif // UTILS_H
# CST435 Assignment 2: Parallel Image Processing

## Project Overview

This project implements a parallel image processing system that applies five different filters to food images. We process 300 food images from the Food-101 dataset using both sequential (single-thread) and parallel programming approaches to demonstrate the performance benefits and limitations of parallel computing.

## Team Members (Group 34)

| Name | Matric No. | Email |
|------|------------|-------|
| Muhammad Amin Nasyriq Bin Azlin | 163177 | amin.nsyrq@student.usm.my |
| Mahdil Ashronie Bin Muhamad Murtadza | 163966 | mahdilash@student.usm.my |
| Khalil Ahmad Bin Zainalabidin | 164416 | khalilahmad@student.usm.my |
| Jalal Naim Bin Mat Yaacob | 164498 | jalalnaim@student.usm.my |

### Project Objectives

- Implement image processing using sequential programming as a baseline
- Create parallel versions using two different C++ approaches: `std::thread` and OpenMP
- Deploy and test the applications on Google Cloud Platform
- Compare performance using speedup and efficiency metrics
- Analyze the practical limitations of parallel computing (Amdahl's Law)

### Dataset
- **Source**: Food-101 Dataset (Kaggle)
- **Subset Size**: 300 images (3 images from 100 types of food variant from Food-101 Dataset)
- **Total Size**: More than 13 MB
- **Selection**: Random sampling across food categories
- **Format**: JPG images, ~512x384 pixels average

### Image Filters Implemented
1. **Grayscale Conversion** - RGB to grayscale using luminance formula
2. **Gaussian Blur** - 3×3 Gaussian kernel smoothing
3. **Edge Detection** - Sobel filter
4. **Image Sharpening** - Edge enhancement
5. **Brightness Adjustment** - Intensity modification

## Technologies Used

### Programming Languages & Libraries
- **C++17** - Main programming language
- **stb_image** - Lightweight library for loading JPG images
- **stb_image_write** - Library for saving processed images
- **std::thread** - C++ standard library for manual thread management
- **OpenMP** - Compiler directives for automatic parallelization

### Build Tools
- **g++** - C++ compiler
- **CMake** - Build system generator
- **Make** - Build automation tool

### Cloud Platform
- **Google Cloud Platform (GCP)** - Virtual machine hosting
- **Instance Type**: e2-standard-8 (8 vCPUs, 32 GB RAM)
- **Operating System**: Debian GNU/Linux 12

## Getting Started

### Prerequisites

Make sure you have the following installed:
- g++ compiler (version 7.0 or higher)
- Make
- OpenMP development libraries
- Git

### Installation Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/jalalnaim924/CST435-assignment2.git
   cd CST435-assignment2
   ```

2. **Install required packages** (on Debian/Ubuntu)
   ```bash
   sudo apt-get update
   sudo apt-get install -y build-essential cmake git libomp-dev
   ```

3. **Navigate to implementation directory**
   ```bash
   cd cpp_implementation
   ```

4. **Build the project**
   ```bash
   make clean
   make all
   ```

5. **Verify executables were created**
   ```bash
   ls -lh bin/
   ```
   You should see three executables:
   - `image_processor_sequential`
   - `image_processor_threads`
   - `image_processor_openmp`

## Running the Programs

### Sequential Version (Baseline)
```bash
./bin/image_processor_sequential
```
This processes all 300 images one at a time using a single thread.

### Parallel Version with std::thread
```bash
./bin/image_processor_threads
```
This automatically tests with 1, 2, 4, and 8 threads, creating separate output folders for each configuration.

### Parallel Version with OpenMP
```bash
./bin/image_processor_openmp
```
This also tests with 1, 2, 4, and 8 threads, similar to the std::thread version.

## Performance Results
Execution Time Comparison
- [results\execution_time_comparison.png]

Efficiency Comparison
- [results\efficiency_comparison.png]


## Project Video

https://youtu.be/tYn16NPASzk?si=Ftj6jvny-RJ5Xm1_

**© 2026 CST435 Assignment 2 Group 34. All rights reserved.**
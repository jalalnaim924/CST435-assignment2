# CST435 Parallel and Cloud Computing
## C++ Implementation: std::thread vs OpenMP

### Project Overview
Parallel image processing system implementing 5 image filters using two C++ parallelization paradigms, deployed and tested on Google Cloud Platform.

### Group Members
1. Muhammad Amin Nasyriq Bin Azlin - 163177 - Core Logic & Sequential Code
2. Khalil Ahmad Bin Zainalabidin - 164416 - Concurrency Implementation
3. Jalal Naim Bin Mat Yaacob - 164498 - Deployment & Automation
4. Mahdil Ashronie Bin Muhamad Murtadza - 163966 - Metrics & Documentation

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

### Parallel Implementations

#### Paradigm A: std::thread
- Manual thread management
- Custom work distribution
- Explicit synchronization

#### Paradigm B: OpenMP
- Compiler-directive based parallelization
- Automatic work sharing
- Built-in thread management

### System Requirements
- **OS**: Debian GNU/Linux 12 (bookworm)
- **Compiler**: GCC 11+ with C++17 support
- **Libraries**:  stb_image.h and stbi_image_write.h , OpenMP
- **Hardware**: Minimum 4 CPU cores recommended
- **Memory**: 8GB+ recommended

### Installation

#### On GCP VM
```bash
# Clone repository
git clone https://github.com/jalalnaim924/CST435-assignment2.git
cd CST435-assignment2

# Ensure OpenMP is installed on Debian
sudo apt-get update
sudo apt-get install -y libomp-dev build-essential cmake
```

#### Build C++ Implementation
```bash
cd cpp_implementation

# Using Makefile
make clean
make all

# Or using CMake
mkdir build && cd build
cmake ..
make
```

### Usage

#### Automated Testing 
```bash
cd scripts
./run_all_tests.sh
```
#### Program Execution
Navigate to the implementation directory:
```bash 
cd cpp_implementation

# Sequential Implementation 
./bin/image_processor_sequential

# std::thread Implementation (Automatically tests 1, 2, 4, and 8 threads) 
./bin/image_processor_threads

# OpenMP Implementation (Automatically tests 1, 2, 4, and 8 threads) 
./bin/image_processor_openmp

#### Quick Smoke Test
```bash
cd scripts
./quick_test.sh
```

### Performance Results

| Paradigm  |  1 Thread | 2 Thread  | 4 Thread  | 8 Thread  |
|---|---|---|---|---|
|  std::thread Time |  44,148 ms | 37,573 ms  | 34,128 ms  |  34,659 ms |
| OpenMP Time  |  45,748 ms | 38,112 ms  | 34,549 ms  | 37,349 ms  |
|  Sequential Base | 38,193 ms  | -  | -  | -  |

| Threads | std::thread Speedup | OpenMP Speedup | 
|---|---|---|
 | 1 | 1.00x | 1.00x | 
 | 2 | 1.17x | 1.20x |
 | 4 | 1.29x | 1.32x | 
 | 8 | 1.27x | 1.22x |

Parallelization is limited by Amdahl's Law because sequential Disk I/O (loading/saving images) creates a fundamental bottleneck
 Note that efficiency dropped from 100% to 15.25% at 8 threads.


#### Test Environment
- **VM Type**: e2-standard-8 (8 vCPUs, 32 GB Memory)
- **Dataset**: 300 images from Food-101
- **Thread Counts**: 1, 2, 4, 8


### Project Structure
CST435-assignment2/
├── .gitignore
├── README.md
├── cpp_implementation/
│   ├── CMakeLists.txt
│   ├── Makefile
│   ├── bin/                  # Generated: final executables 
│   ├── obj/                  # Generated: object files 
│   ├── include/              # stb_image header-only libraries 
│   │   ├── stb_image.h
│   │   └── stb_image_write.h
│   └── src/                  # Source files and project headers 
│       ├── filters.cpp
│       ├── filters.h
│       ├── image_io.cpp
│       ├── image_io.h
│       ├── main.cpp
│       ├── openmp_version.cpp
│       ├── threads_version.cpp
│       └── utils.h
├── data/
│   ├── input/
│   │   └── food_samples/     # 300 test images from Food-101 
│   │       ├── 10044.jpg
│   │       ├── ... (300 items)
│   └── output/               # Generated processed results 
│       ├── sequential/
│       ├── cpp_threads/
│       └── cpp_openmp/
└── results/                  # Performance data and graphs 
    ├── efficiency_comparison.png
    └── execution_time_comparison.png

### Links
- **GitHub Repository**: [(https://github.com/jalalnaim924/CST435-assignment2.git)]
- **YouTube Demonstration**: [Link]
- **Performance Report**: 
[results\efficiency_comparison.png]
[results\execution_time_comparison.png]

### License
Academic project for CST435 - Parallel and Cloud Computing, Universiti Sains Malaysia

### Acknowledgments
- Food-101 Dataset: Bossard et al., 2014
- Google Cloud Platform for compute resources
- stb_image library: Sean Barrett (nothings/stb)

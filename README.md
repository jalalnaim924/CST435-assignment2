# CST435 Parallel Image Processing System
## C++ Implementation: std::thread vs OpenMP

### Project Overview
Parallel image processing system implementing 5 image filters using two C++ parallelization paradigms, deployed and tested on Google Cloud Platform.

### Group Members
1. Amin - 163177 - Core Logic & Sequential Code
2. Khalil - 164416 - Concurrency Implementation
3. Jalal - 164498 - Deployment & Automation
4. Mahdil - 163966 - Metrics & Documentation

### Dataset
- **Source**: Food-101 Dataset (Kaggle)
- **Subset Size**: 300 images (3 images from 100 types of food variant from Food-101 Dataset)
- **Total Size**: 4.29 MB (4,501,694 bytes)
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
- **OS**: Ubuntu 22.04 LTS
- **Compiler**: GCC 11+ with C++17 support
- **Libraries**: OpenCV 4.x, OpenMP
- **Hardware**: Minimum 4 CPU cores recommended
- **Memory**: 8GB+ recommended

### Installation

#### On GCP VM
```bash
# Clone repository
git clone https://github.com/jalalnaim924/CST435-assignment2.git
cd CST435-Parallel-Image-Processing

# Install C++ dependencies
chmod +x setup/install_cpp_dependencies.sh
./setup/install_cpp_dependencies.sh

# Upload dataset (from local machine)
# See setup/prepare_dataset.sh for instructions
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

#### Automated Testing (Recommended)
```bash
cd scripts
./run_all_tests.sh
```

This will:
- Test both paradigms with 1, 2, 4, 8 threads
- Process all 300 images for each configuration
- Generate performance CSV and graphs
- Create comprehensive analysis report

#### Manual Execution

**std::thread version:**
```bash
cd cpp_implementation
./bin/image_processor_threads \
    --images ../data/input/food_samples \
    --threads 4 \
    --output ../data/output/cpp_threads/4_thread
```

**OpenMP version:**
```bash
export OMP_NUM_THREADS=4
./bin/image_processor_openmp \
    --images ../data/input/food_samples \
    --threads 4 \
    --output ../data/output/cpp_openmp/4_thread
```

#### Quick Smoke Test
```bash
cd scripts
./quick_test.sh
```

### Performance Results

#### Test Environment
- **VM Type**: e2-standard-4 (4 vCPUs, 16GB RAM)
- **Dataset**: 300 images from Food-101
- **Thread Counts**: 1, 2, 4, 8

#### Summary Results
[Results will be added after testing]

See `results/performance_report.md` for detailed analysis.

### Project Structure
├── cpp_implementation/     # C++ source code
│   ├── src/               # Source files
│   ├── bin/               # Compiled executables
│   └── Makefile           # Build configuration
├── data/                  # Dataset and outputs
│   ├── input/            # 300 test images
│   └── output/           # Processed results
├── scripts/              # Automation scripts
├── results/              # Performance data and graphs
└── docs/                 # Documentation

### Links
- **GitHub Repository**: [(https://github.com/jalalnaim924/CST435-assignment2.git)]
- **YouTube Demonstration**: [Link]
- **Performance Report**: [results/performance_report.md]

### License
Academic project for CST435 - Parallel and Cloud Computing, Universiti Sains Malaysia

### Acknowledgments
- Food-101 Dataset: Bossard et al., 2014
- OpenCV Library: opencv.org
- Google Cloud Platform for compute resources

#!/bin/bash
# File: setup/install_cpp_dependencies.sh

echo "=========================================="
echo "Installing C++ Dependencies for CST435"
echo "=========================================="

# Update system
echo "Updating system packages..."
sudo apt-get update
sudo apt-get upgrade -y

# Install essential build tools
echo "Installing build tools..."
sudo apt-get install -y build-essential
sudo apt-get install -y cmake
sudo apt-get install -y git
sudo apt-get install -y wget

# Install OpenMP (comes with GCC, but ensure it's available)
echo "Installing OpenMP support..."
sudo apt-get install -y libomp-dev

# Install OpenCV for image processing
echo "Installing OpenCV..."
sudo apt-get install -y libopencv-dev
sudo apt-get install -y libopencv-contrib-dev

# Install additional useful tools
sudo apt-get install -y htop
sudo apt-get install -y tree
sudo apt-get install -y bc  # for floating point calculations in scripts

# Verify installations
echo ""
echo "=========================================="
echo "Verification:"
echo "=========================================="
echo "GCC Version:"
g++ --version | head -n 1

echo ""
echo "CMake Version:"
cmake --version | head -n 1

echo ""
echo "OpenMP Support:"
echo '#include <omp.h>' | g++ -fopenmp -x c++ -E - > /dev/null 2>&1 && echo "✓ OpenMP available" || echo "✗ OpenMP not found"

echo ""
echo "OpenCV Version:"
pkg-config --modversion opencv4 2>/dev/null || pkg-config --modversion opencv

echo ""
echo "=========================================="
echo "Installation Complete!"
echo "=========================================="
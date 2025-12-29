#!/bin/bash
# File: scripts/quick_test.sh
# Quick smoke test with just a few images

echo "Quick Test - C++ Implementation"
echo "================================"

cd ../cpp_implementation

# Build both versions
echo "Building..."
make clean && make all

if [ $? -ne 0 ]; then
    echo "✗ Build failed"
    exit 1
fi
echo "✓ Build successful"

# Test with 2 threads on first 5 images
TEST_INPUT="../data/input/food_samples"
TEST_OUTPUT="/tmp/quick_test_output"
mkdir -p "$TEST_OUTPUT"

echo ""
echo "Testing std::thread version (2 threads, 5 images)..."
./bin/image_processor_threads \
    --images "$TEST_INPUT" \
    --threads 2 \
    --output "$TEST_OUTPUT/threads" \
    --limit 5

if [ $? -eq 0 ]; then
    echo "✓ std::thread version works"
else
    echo "✗ std::thread version failed"
fi

echo ""
echo "Testing OpenMP version (2 threads, 5 images)..."
export OMP_NUM_THREADS=2
./bin/image_processor_openmp \
    --images "$TEST_INPUT" \
    --threads 2 \
    --output "$TEST_OUTPUT/openmp" \
    --limit 5

if [ $? -eq 0 ]; then
    echo "✓ OpenMP version works"
else
    echo "✗ OpenMP version failed"
fi

echo ""
echo "Quick test complete!"
rm -rf "$TEST_OUTPUT"

cd ../scripts
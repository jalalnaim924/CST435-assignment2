#!/bin/bash
# File: scripts/run_all_tests.sh

set -e  # Exit on error

echo "================================================"
echo "CST435 Parallel Image Processing - Automated Tests"
echo "C++ Implementation (std::thread + OpenMP)"
echo "================================================"
echo ""

# Configuration
THREAD_COUNTS=(1 2 4 8)
INPUT_DIR="../data/input/food_samples"
RESULTS_DIR="../results"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)

# Count images
NUM_IMAGES=$(ls -1 $INPUT_DIR/*.jpg 2>/dev/null | wc -l)
echo "Test Configuration:"
echo "  - Images to process: $NUM_IMAGES"
echo "  - Thread counts: ${THREAD_COUNTS[*]}"
echo "  - VM CPUs: $(nproc)"
echo ""

# Create results directory
mkdir -p "$RESULTS_DIR"

# Initialize results file
RESULTS_FILE="$RESULTS_DIR/performance_data_${TIMESTAMP}.csv"
echo "Implementation,Paradigm,Threads,ExecutionTime(s),ImagesProcessed,TimePerImage(s),Speedup,Efficiency" > "$RESULTS_FILE"

echo "Starting automated tests at $(date)"
echo ""

# Test std::thread Implementation
echo "=== Testing std::thread Implementation ==="
./run_threads_tests.sh "$RESULTS_FILE"
echo ""

# Test OpenMP Implementation
echo "=== Testing OpenMP Implementation ==="
./run_openmp_tests.sh "$RESULTS_FILE"
echo ""

# Generate performance report
echo "=== Generating Performance Report ==="
python3 generate_performance_report.py "$RESULTS_FILE"

echo ""
echo "================================================"
echo "All tests completed at $(date)"
echo "Results saved to: $RESULTS_FILE"
echo "Graphs generated in: $RESULTS_DIR/"
echo "================================================"
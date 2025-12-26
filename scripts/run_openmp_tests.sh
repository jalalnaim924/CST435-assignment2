# Test OpenMP version

RESULTS_FILE=$1
THREAD_COUNTS=(1 2 4 8)
INPUT_DIR="../data/input/food_samples"

cd ../cpp_implementation

# Build OpenMP version
echo "Building OpenMP implementation..."
make openmp

if [ ! -f "bin/image_processor_openmp" ]; then
    echo "ERROR: Failed to build OpenMP version"
    exit 1
fi

# Count images
NUM_IMAGES=$(ls -1 $INPUT_DIR/*.jpg 2>/dev/null | wc -l)

# Test OpenMP version
echo "Testing OpenMP implementation with $NUM_IMAGES images..."
for threads in "${THREAD_COUNTS[@]}"; do
    echo "  Running with $threads thread(s)..."
    
    OUTPUT_DIR="../data/output/cpp_openmp/${threads}_thread"
    mkdir -p "$OUTPUT_DIR"
    
    # Set OMP_NUM_THREADS environment variable
    export OMP_NUM_THREADS=$threads
    
    # Run and time execution
    START=$(date +%s.%N)
    ./bin/image_processor_openmp \
        --images "$INPUT_DIR" \
        --threads "$threads" \
        --output "$OUTPUT_DIR" \
        > /dev/null 2>&1
    END=$(date +%s.%N)
    
    EXEC_TIME=$(echo "$END - $START" | bc)
    TIME_PER_IMAGE=$(echo "scale=4; $EXEC_TIME / $NUM_IMAGES" | bc)
    
    echo "    Execution time: ${EXEC_TIME}s (${TIME_PER_IMAGE}s per image)"
    
    # Write to results
    echo "C++,OpenMP,$threads,$EXEC_TIME,$NUM_IMAGES,$TIME_PER_IMAGE,0,0" >> "$RESULTS_FILE"
done

cd ../scripts
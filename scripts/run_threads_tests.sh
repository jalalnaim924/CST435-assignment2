# test std::thread version

RESULTS_FILE=$1
THREAD_COUNTS=(1 2 4 8)
INPUT_DIR="../data/input/food_samples"

cd ../cpp_implementation

# Build std::thread version
echo "Building std::thread implementation..."
make clean
make threads

if [ ! -f "bin/image_processor_threads" ]; then
    echo "ERROR: Failed to build threads version"
    exit 1
fi

# Count images
NUM_IMAGES=$(ls -1 $INPUT_DIR/*.jpg 2>/dev/null | wc -l)

# Test std::thread version
echo "Testing std::thread implementation with $NUM_IMAGES images..."
for threads in "${THREAD_COUNTS[@]}"; do
    echo "  Running with $threads thread(s)..."
    
    OUTPUT_DIR="../data/output/cpp_threads/${threads}_thread"
    mkdir -p "$OUTPUT_DIR"
    
    # Run and time execution
    START=$(date +%s.%N)
    ./bin/image_processor_threads \
        --images "$INPUT_DIR" \
        --threads "$threads" \
        --output "$OUTPUT_DIR" \
        > /dev/null 2>&1
    END=$(date +%s.%N)
    
    EXEC_TIME=$(echo "$END - $START" | bc)
    TIME_PER_IMAGE=$(echo "scale=4; $EXEC_TIME / $NUM_IMAGES" | bc)
    
    echo "    Execution time: ${EXEC_TIME}s (${TIME_PER_IMAGE}s per image)"
    
    # Write to results (speedup/efficiency calculated later)
    echo "C++,std::thread,$threads,$EXEC_TIME,$NUM_IMAGES,$TIME_PER_IMAGE,0,0" >> "$RESULTS_FILE"
done

cd ../scripts
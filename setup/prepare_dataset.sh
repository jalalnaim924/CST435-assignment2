#!/bin/bash
# File: setup/prepare_dataset.sh

echo "Preparing Food-101 subset for project..."

# Create data directories
mkdir -p data/input/food_samples
mkdir -p data/output/{cpp_threads,cpp_openmp}

# Download Food-101 dataset
# Option 1: Download full dataset and extract subset locally
# Option 2: Download from Kaggle directly on GCP

echo "Dataset preparation options:"
echo "1. Download 300 images from Kaggle (requires Kaggle API)"
echo "2. Upload pre-selected subset from local machine"
echo ""
echo "Recommended: Select 300 images locally, then upload to GCP"
#!/usr/bin/env python3
# File: setup/select_food101_subset.py
# Run this LOCALLY to select 300 images from Food-101

import os
import shutil
import random
from pathlib import Path

def select_food101_subset(food101_path, output_path, num_images=300):
    """
    Select a random subset of images from Food-101 dataset
    
    Args:
        food101_path: Path to Food-101 'images' directory
        output_path: Where to save selected images
        num_images: Number of images to select (default: 300)
    """
    
    # Create output directory
    output_dir = Path(output_path)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # Get all image paths
    food101_dir = Path(food101_path)
    all_images = list(food101_dir.glob('**/*.jpg'))
    
    print(f"Found {len(all_images)} total images in Food-101")
    
    # Randomly select images
    selected_images = random.sample(all_images, min(num_images, len(all_images)))
    
    # Copy selected images
    print(f"Copying {len(selected_images)} images to {output_path}...")
    
    for i, img_path in enumerate(selected_images, 1):
        # Create simpler filename: food101_001.jpg, food101_002.jpg, etc.
        new_name = f"food101_{i:03d}.jpg"
        dest_path = output_dir / new_name
        
        shutil.copy2(img_path, dest_path)
        
        if i % 50 == 0:
            print(f"  Copied {i}/{len(selected_images)} images...")
    
    print(f"✓ Successfully prepared {len(selected_images)} images")
    print(f"Total size: {sum(f.stat().st_size for f in output_dir.glob('*.jpg')) / (1024*1024):.2f} MB")

if __name__ == "__main__":
    # Adjust these paths to your local setup
    food101_path = "/path/to/food-101/images"  # Change this
    output_path = "./food_samples_300"
    
    select_food101_subset(food101_path, output_path, num_images=300)
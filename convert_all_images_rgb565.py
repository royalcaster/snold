#!/usr/bin/env python3
"""
Batch Image Converter for RGB565 Format
Converts all images in assets/img to RGB565 C arrays for SSD1351 color display
"""

import os
import subprocess
from PIL import Image

# Configuration
image_folder = 'assets/img'
output_folder = 'include'

def convert_all_images():
    """Convert all images to RGB565 format using the working convert_image.py script"""
    
    if not os.path.exists(image_folder):
        print(f"Error: Image folder '{image_folder}' not found.")
        return

    if not os.path.exists(output_folder):
        print(f"Creating output folder '{output_folder}'...")
        os.makedirs(output_folder)

    print(f"Converting images from {image_folder} to {output_folder}")
    print("Target format: RGB565 (16-bit color)")
    
    converted_count = 0
    failed_count = 0
    
    for filename in os.listdir(image_folder):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            print(f"\n--- Converting {filename} ---")
            
            image_path = os.path.join(image_folder, filename)
            
            # Generate output filename 
            base_name = os.path.splitext(filename)[0].lower()
            output_filename = f'img_{base_name}_rgb565.h'
            output_path = os.path.join(output_folder, output_filename)
            
            try:
                # Use the working convert_image.py script
                result = subprocess.run([
                    'python3', 'convert_image.py', 
                    image_path, 
                    output_path
                ], capture_output=True, text=True)
                
                if result.returncode == 0:
                    print(f"✅ Successfully converted {filename}")
                    converted_count += 1
                else:
                    print(f"❌ Failed to convert {filename}")
                    print(f"Error: {result.stderr}")
                    failed_count += 1
                    
            except Exception as e:
                print(f"❌ Exception converting {filename}: {e}")
                failed_count += 1
    
    print(f"\n=== Conversion Complete ===")
    print(f"✅ Successfully converted: {converted_count} images")
    print(f"❌ Failed conversions: {failed_count} images")
    print(f"📁 Output directory: {output_folder}")

if __name__ == '__main__':
    convert_all_images() 
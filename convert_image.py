#!/usr/bin/env python3
"""
Image to C Array Converter for ESP32 OLED Display
Converts PNG/JPG images to RGB565 C arrays for Adafruit SSD1351 display
"""

from PIL import Image
import sys
import os

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format"""
    r = (r >> 3) & 0x1F
    g = (g >> 2) & 0x3F
    b = (b >> 3) & 0x1F
    return (r << 11) | (g << 5) | b

def image_to_c_array(image_path, output_path=None, max_width=128, max_height=128):
    """Convert image to C array"""
    
    # Open and resize image
    img = Image.open(image_path)
    
    # Convert to RGB if needed
    if img.mode != 'RGB':
        img = img.convert('RGB')
    
    # Resize if too large
    if img.width > max_width or img.height > max_height:
        img.thumbnail((max_width, max_height), Image.Resampling.LANCZOS)
    
    width, height = img.size
    
    # Generate C array
    c_array = f"// Converted from: {os.path.basename(image_path)}\n"
    c_array += f"// Size: {width}x{height} pixels\n"
    c_array += f"const uint16_t {os.path.splitext(os.path.basename(image_path))[0]}[] = {{\n"
    
    pixels = []
    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            rgb565 = rgb888_to_rgb565(r, g, b)
            pixels.append(f"0x{rgb565:04X}")
    
    # Format output with 16 pixels per line
    for i in range(0, len(pixels), 16):
        line_pixels = pixels[i:i+16]
        c_array += "  " + ", ".join(line_pixels) + ",\n"
    
    c_array += "};\n"
    
    # Write to file or print
    if output_path:
        with open(output_path, 'w') as f:
            f.write(c_array)
        print(f"Converted {image_path} to {output_path}")
        print(f"Image size: {width}x{height} pixels")
        print(f"Array size: {len(pixels)} elements")
    else:
        print(c_array)
    
    return width, height

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 convert_image.py <image_path> [output_path]")
        print("Example: python3 convert_image.py logo.png logo_array.h")
        return
    
    image_path = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) > 2 else None
    
    if not os.path.exists(image_path):
        print(f"Error: Image file '{image_path}' not found")
        return
    
    try:
        width, height = image_to_c_array(image_path, output_path)
        print(f"Successfully converted {image_path} ({width}x{height})")
    except Exception as e:
        print(f"Error converting image: {e}")

if __name__ == "__main__":
    main() 
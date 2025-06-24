import os
from PIL import Image

# Configuration
image_folder = 'assets/img'
output_folder = 'include'
image_size = (128, 128)  # EXACT target size - no aspect ratio preservation

def convert_image_to_c_array(image_path, output_path):
    """Converts an image to a C header file with a byte array."""
    try:
        # Open the image
        img = Image.open(image_path)
        print(f"Original size: {img.size}")
        
        # CRITICAL FIX: Use resize() instead of thumbnail() to force exact dimensions
        # This ensures 500x500 becomes exactly 128x128, not smaller due to aspect ratio
        img = img.resize(image_size, Image.Resampling.LANCZOS)
        print(f"Resized to: {img.size}")
        
        # Convert to grayscale first for better quality control
        img = img.convert('L')  # Grayscale (0-255)
        
        # Apply better thresholding instead of direct '1' conversion
        # This gives more control over the black/white conversion
        threshold = 128  # Adjust this value (0-255) to control white/black balance
        img = img.point(lambda x: 255 if x > threshold else 0, mode='1')

        # Get image dimensions (should be exactly 128x128 now)
        width, height = img.size
        print(f"Final size: {width}x{height}")
        
        # Get pixel data
        pixels = list(img.getdata())
        
        # C-style variable name from filename
        base_name = os.path.basename(image_path)
        variable_name = 'img_' + os.path.splitext(base_name)[0].lower()

        # Create C header content
        c_code = f'#ifndef _{variable_name.upper()}_H_\n'
        c_code += f'#define _{variable_name.upper()}_H_\n\n'
        c_code += '#include <pgmspace.h>\n\n'
        c_code += f'// Image: {base_name}\n'
        c_code += f'// Original size: Input image\n'
        c_code += f'// Processed size: {width}x{height}\n'
        c_code += f'// Format: 1-bit monochrome bitmap\n\n'
        c_code += f'const unsigned char {variable_name}[] PROGMEM = {{\n    '
        
        byte_array = []
        # Process the image in vertical 8-pixel columns (pages) for SSD1351
        for x in range(width):
            for y in range(0, height, 8):
                byte = 0
                for bit in range(8):
                    if (y + bit) < height:
                        pixel_index = (y + bit) * width + x
                        # If pixel is white (255), set the bit
                        if pixels[pixel_index] > 0:
                            byte |= (1 << bit)
                byte_array.append(byte)

        # Format the byte array into the C code
        for i, byte in enumerate(byte_array):
            c_code += f'0x{byte:02X}, '
            if (i + 1) % 16 == 0:
                c_code += '\n    '
        
        # Remove trailing comma and space
        if c_code.endswith(', '):
            c_code = c_code[:-2]
            
        c_code += '\n};\n\n'
        c_code += f'#endif // _{variable_name.upper()}_H_\n'

        # Write to the output file
        with open(output_path, 'w') as f:
            f.write(c_code)
        
        print(f'Successfully converted {image_path} to {output_path}')
        print(f'Array size: {len(byte_array)} bytes for {width}x{height} pixels')

    except Exception as e:
        print(f'Error converting {image_path}: {e}')
        raise e  # Re-raise to see full error details

def main():
    """Main function to convert all images."""
    if not os.path.exists(image_folder):
        print(f"Error: Image folder '{image_folder}' not found.")
        return

    if not os.path.exists(output_folder):
        print(f"Creating output folder '{output_folder}'...")
        os.makedirs(output_folder)

    print(f"Converting images from {image_folder} to {output_folder}")
    print(f"Target size: {image_size}")
    
    converted_count = 0
    for filename in os.listdir(image_folder):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            print(f"\n--- Converting {filename} ---")
            image_path = os.path.join(image_folder, filename)
            output_filename = 'img_' + os.path.splitext(filename)[0].lower() + '.h'
            output_path = os.path.join(output_folder, output_filename)
            
            try:
                convert_image_to_c_array(image_path, output_path)
                converted_count += 1
            except Exception as e:
                print(f"FAILED to convert {filename}: {e}")
    
    print(f"\n=== Conversion Complete ===")
    print(f"Successfully converted {converted_count} images")

if __name__ == '__main__':
    main() 
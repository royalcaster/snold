import os
from PIL import Image

# Configuration
image_folder = 'assets/img'
output_folder = 'include'
image_size = (128, 128)
# For 1-bit monochrome, each byte holds 8 pixels
# Display is monochrome, 1 bit per pixel
# For SSD1306/SH1106, data is sent in pages (8 rows of pixels)
# So we process the image in vertical byte chunks

def convert_image_to_c_array(image_path, output_path):
    """Converts an image to a C header file with a byte array."""
    try:
        # Open and resize the image
        img = Image.open(image_path)
        img = img.resize(image_size, Image.Resampling.LANCZOS)
        # Convert to 1-bit monochrome
        img = img.convert('1')

        # Get image dimensions
        width, height = img.size
        
        # Invert colors if needed (often white is 1, black is 0)
        # This depends on your display library. Let's assume white=1 for now.
        pixels = list(img.getdata())
        
        # C-style variable name from filename
        base_name = os.path.basename(image_path)
        variable_name = 'img_' + os.path.splitext(base_name)[0].lower()

        # Create C header content
        c_code = f'#ifndef _{variable_name.upper()}_H_\n'
        c_code += f'#define _{variable_name.upper()}_H_\n\n'
        c_code += '#include <pgmspace.h>\n\n'
        c_code += f'// Image: {base_name}\n'
        c_code += f'// Size: {width}x{height}\n\n'
        c_code += f'const unsigned char {variable_name}[] PROGMEM = {{\n    '
        
        byte_array = []
        # Process the image in vertical 8-pixel columns (pages)
        for x in range(width):
            for y in range(0, height, 8):
                byte = 0
                for bit in range(8):
                    if (y + bit) < height:
                        pixel_index = (y + bit) * width + x
                        # If pixel is not black (i.e., it's white), set the bit
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

    except Exception as e:
        print(f'Error converting {image_path}: {e}')

def main():
    """Main function to convert all images."""
    if not os.path.exists(image_folder):
        print(f"Error: Image folder '{image_folder}' not found.")
        return

    if not os.path.exists(output_folder):
        print(f"Creating output folder '{output_folder}'...")
        os.makedirs(output_folder)

    for filename in os.listdir(image_folder):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            image_path = os.path.join(image_folder, filename)
            output_filename = 'img_' + os.path.splitext(filename)[0].lower() + '.h'
            output_path = os.path.join(output_folder, output_filename)
            convert_image_to_c_array(image_path, output_path)

if __name__ == '__main__':
    main() 
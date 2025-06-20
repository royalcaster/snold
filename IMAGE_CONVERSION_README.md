# 🖼️ Image Display for ESP32 OLED

This guide shows you how to display images on your ESP32 OLED screen (SSD1351).

## 📋 Quick Start

### 1. Convert Your PNG to C Array

```bash
# Install Python Pillow if you haven't already
pip install Pillow

# Convert your image
python3 convert_image.py your_image.png your_image_array.h
```

### 2. Add to Your Project

1. **Copy the generated array** to `src/images.cpp`
2. **Add declaration** to `src/images.h`
3. **Use in your code**:

```cpp
// In your main.cpp
showCenteredImage(yourImageArray, width, height);
```

## 🛠️ Methods to Convert Images

### Method 1: Python Script (Recommended)
```bash
python3 convert_image.py logo.png logo_array.h
```

### Method 2: Online Converters
- **Image2cpp**: https://javl.github.io/image2cpp/
- **LCD Assistant**: https://en.radzio.dxp.pl/bitmap_converter/

### Method 3: GIMP/Photoshop
1. Resize image to fit your screen (max 128x128)
2. Export as raw RGB565 format
3. Convert to C array manually

## 📐 Image Specifications

- **Format**: RGB565 (16-bit color)
- **Max Size**: 128x128 pixels (your screen size)
- **Supported**: PNG, JPG, BMP, GIF
- **Transparency**: Black pixels (0x0000) are treated as transparent

## 💡 Usage Examples

### Show Image Centered
```cpp
showCenteredImage(myImage, 64, 64);
```

### Show Image at Specific Position
```cpp
showImage(myImage, 64, 64, 10, 20); // x=10, y=20
```

### Draw Image Without Clearing Screen
```cpp
drawBitmap(&display, myImage, 10, 20, 64, 64);
```

## 🔧 Customization

### Change Image Size Limits
Edit `convert_image.py`:
```python
width, height = image_to_c_array(image_path, output_path, max_width=64, max_height=64)
```

### Add Multiple Images
```cpp
// In images.h
extern const uint16_t logoImage[];
extern const uint16_t iconImage[];

// In images.cpp
const uint16_t logoImage[] = { /* your array */ };
const uint16_t iconImage[] = { /* your array */ };
```

## 🎨 Color Format

Your display uses **RGB565** format:
- **Red**: 5 bits (0-31)
- **Green**: 6 bits (0-63) 
- **Blue**: 5 bits (0-31)

Example colors:
- `0xFFFF` = White
- `0x0000` = Black
- `0xF800` = Red
- `0x07E0` = Green
- `0x001F` = Blue

## 🚀 Performance Tips

1. **Smaller images** = faster loading
2. **Use transparency** (black pixels) to save memory
3. **Pre-convert** images to avoid runtime conversion
4. **Store in PROGMEM** for large images (if needed)

## 📁 File Structure

```
src/
├── images.h          # Image function declarations
├── images.cpp        # Image functions + your arrays
├── main.cpp          # Your main program
└── convert_image.py  # Conversion script
```

## 🔍 Troubleshooting

### Image Not Showing
- Check image dimensions (max 128x128)
- Verify RGB565 format
- Ensure array is properly declared

### Memory Issues
- Reduce image size
- Use fewer colors
- Store in PROGMEM for large images

### Wrong Colors
- Check RGB565 conversion
- Verify color format in converter

## 📚 Advanced Features

### Animation
```cpp
// Show multiple images in sequence
showCenteredImage(frame1, 32, 32);
delay(100);
showCenteredImage(frame2, 32, 32);
delay(100);
```

### Overlay Images
```cpp
// Draw image on top of existing content
drawBitmap(&display, overlayImage, 0, 0, 32, 32);
```

### Dynamic Images
```cpp
// Create images programmatically
uint16_t dynamicImage[64];
// Fill array with pixel data
showImage(dynamicImage, 8, 8, 0, 0);
``` 
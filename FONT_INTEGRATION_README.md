# 🎨 Custom TTF Font Integration

This guide shows you how to integrate your `FATPIXEL.ttf` font into the ESP32 OLED display.

## 🚀 Quick Start

### 1. Convert Your TTF Font

```bash
# Install required Python packages
pip install Pillow

# Convert FATPIXEL.ttf to C arrays
python3 convert_font.py assets/FATPIXEL.ttf src/custom_font.cpp 16
```

### 2. Integrate into Your Project

1. **Add to your main.cpp:**
```cpp
#include "custom_font.h"

// In your setup() or loop():
drawCustomText(&display, "HELLO WORLD", 10, 10, WHITE);
```

## 📋 Step-by-Step Instructions

### Step 1: Convert the Font

```bash
# Navigate to your project directory
cd /home/royalcaster/Documents/GitHub/snold/snold

# Convert FATPIXEL.ttf to 16px size
python3 convert_font.py assets/FATPIXEL.ttf src/custom_font.cpp 16

# Or try different sizes:
python3 convert_font.py assets/FATPIXEL.ttf src/custom_font.cpp 12  # Smaller
python3 convert_font.py assets/FATPIXEL.ttf src/custom_font.cpp 20  # Larger
```

### Step 2: Test the Font

Add this to your `main.cpp`:

```cpp
#include "custom_font.h"

void setup() {
  // ... existing setup code ...
  
  // Test the custom font
  display.fillScreen(BLACK);
  drawCustomText(&display, "FATPIXEL", 10, 20, CYAN);
  drawCustomText(&display, "FONT TEST", 10, 40, GREEN);
  drawCustomText(&display, "1234567890", 10, 60, YELLOW);
}
```

### Step 3: Use in Your Project

```cpp
// Draw single character
drawCustomChar(&display, 'A', 10, 10, WHITE);

// Draw text string
drawCustomText(&display, "Hello World!", 10, 30, RED);

// Draw with different colors
drawCustomText(&display, "Custom Font", 10, 50, CYAN);
```

## 🎛️ Font Options

### Font Sizes
- **12px**: Small, good for labels
- **16px**: Standard, good readability
- **20px**: Large, good for titles
- **24px**: Extra large, for headers

### Character Sets
The converter includes:
- **A-Z**: Uppercase letters
- **a-z**: Lowercase letters  
- **0-9**: Numbers
- **!@#$%^&*()_+-=[]{}|;:,.<>?**: Special characters
- **Space**: Space character

## 🔧 Customization

### Change Character Set
Edit `convert_font.py` line with `chars=` parameter:

```python
chars="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*() "  # Only uppercase + numbers
```

### Add More Characters
```python
chars="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:,.<>?€£¥¢§©®™°±×÷"  # Extended set
```

### Multiple Font Sizes
```bash
# Generate multiple sizes
python3 convert_font.py assets/FATPIXEL.ttf src/fatpixel_12.cpp 12
python3 convert_font.py assets/FATPIXEL.ttf src/fatpixel_16.cpp 16
python3 convert_font.py assets/FATPIXEL.ttf src/fatpixel_20.cpp 20
```

## 📁 Generated Files

After conversion, you'll get:
- `src/custom_font.h` - Header file with declarations
- `src/custom_font.cpp` - Implementation with font data

## 🎨 Usage Examples

### Basic Text Display
```cpp
#include "custom_font.h"

void showCustomText() {
  display.fillScreen(BLACK);
  drawCustomText(&display, "FATPIXEL", 10, 10, WHITE);
  drawCustomText(&display, "ROCKS!", 10, 30, CYAN);
}
```

### Centered Text
```cpp
void showCenteredText(const char* text, uint16_t color) {
  // Calculate text width (approximate)
  int textWidth = strlen(text) * FONT_SIZE;
  int x = (OLED_WIDTH - textWidth) / 2;
  int y = (OLED_HEIGHT - FONT_SIZE) / 2;
  
  drawCustomText(&display, text, x, y, color);
}
```

### Animated Text
```cpp
void animateText(const char* text) {
  for (int i = 0; i < strlen(text); i++) {
    char singleChar[2] = {text[i], '\0'};
    drawCustomText(&display, singleChar, 10 + i * FONT_SIZE, 20, WHITE);
    delay(100);
  }
}
```

## 🔍 Troubleshooting

### Font Not Loading
- Check font file path: `assets/FATPIXEL.ttf`
- Verify font file is valid TTF
- Check Python Pillow installation

### Characters Missing
- Verify character is in the `chars` string
- Check font supports the character
- Try different font size

### Memory Issues
- Reduce font size (12px instead of 20px)
- Reduce character set
- Use PROGMEM for large fonts

### Wrong Characters
- Check character mapping in `getCharIndex()`
- Verify font encoding
- Test with simple characters first

## 🚀 Performance Tips

1. **Smaller fonts** = faster rendering
2. **Fewer characters** = less memory usage
3. **Pre-calculate positions** for static text
4. **Use PROGMEM** for large font data

## 📚 Advanced Features

### Variable Width Support
The generated font supports variable character widths (each character can have different width).

### Transparency
Black pixels (0x0000) are treated as transparent.

### Color Customization
Each character can be drawn in any RGB565 color.

### Multiple Fonts
You can generate multiple font files and switch between them:

```cpp
#include "fatpixel_12.h"
#include "fatpixel_16.h"

// Use different sizes
drawCustomText(&display, "Small", 10, 10, WHITE);  // 12px
drawCustomText(&display, "Large", 10, 30, CYAN);   // 16px
``` 
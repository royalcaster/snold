# ESP32 Throw Detection Device

A smart device that measures throw height using barometric pressure sensing and displays results on a color OLED screen with animated graphics.

**University Project**  
Applied Computer Science (Master's Program)  
HTW Dresden

## 📋 Project Overview

This project implements a portable throw detection system using an ESP32 microcontroller. The device continuously monitors altitude changes through a high-precision barometric sensor and automatically detects when an object (or the device itself) is thrown into the air. The measured throw height is displayed on a vibrant 128x128 color OLED screen alongside animated graphics.

### Key Features

- **Automatic Throw Detection**: Uses BMP388 barometric sensor for precise altitude measurement
- **Real-time Display**: 128x128 color OLED with smooth animations
- **Smart Measurement**: Displays results in centimeters or meters based on throw height
- **Animated Interface**: Cycling animations showing different activities (walk, play, throw, rocket, shake)
- **High Performance**: Optimized rendering with DMA acceleration and multiple drawing methods

## 🛠 Hardware Requirements

### Core Components
- **ESP32 Development Board** (ESP32-WROOM-32 or similar)
- **BMP388 Barometric Pressure Sensor**
- **SSD1351 128x128 Color OLED Display**

### Connections

| Component | ESP32 Pin | Purpose |
|-----------|-----------|---------|
| **BMP388** | | |
| VCC | 3.3V | Power |
| GND | GND | Ground |
| SDA | GPIO 21 | I2C Data |
| SCL | GPIO 22 | I2C Clock |
| **SSD1351** | | |
| VCC | 3.3V | Power |
| GND | GND | Ground |
| CS | GPIO 5 | Chip Select |
| DC | GPIO 27 | Data/Command |
| RST | GPIO 4 | Reset |
| MOSI | GPIO 23 | SPI Data |
| SCLK | GPIO 18 | SPI Clock |
| **Button** | | |
| Button | GPIO 26 | User Input |
| GND | GND | Ground |

## 💻 Software Setup

### Prerequisites

1. **Visual Studio Code** with **PlatformIO Extension**
2. **Git** for cloning the repository

### Installation Steps

1. **Clone the Repository**
   ```bash
   git clone <repository-url>
   cd snold
   ```

2. **Open in VS Code**
   ```bash
   code .
   ```
   Or open VS Code and use `File > Open Folder` to select the project directory.

3. **Install PlatformIO Extension** (if not already installed)
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Install the extension
   - Restart VS Code if prompted

### Building and Uploading

1. **Connect ESP32** to your computer via USB

2. **Build the Project**
   - Open PlatformIO sidebar (click PlatformIO icon)
   - Under "PROJECT TASKS" → "esp32dev" → "General"
   - Click "Build"
   
   *Alternative: Use keyboard shortcut `Ctrl+Alt+B`*

3. **Upload to ESP32**
   - In PlatformIO sidebar: "PROJECT TASKS" → "esp32dev" → "General"
   - Click "Upload"
   
   *Alternative: Use keyboard shortcut `Ctrl+Alt+U`*

4. **Monitor Serial Output** (optional)
   - In PlatformIO sidebar: "PROJECT TASKS" → "esp32dev" → "General"
   - Click "Monitor"
   
   *Alternative: Use keyboard shortcut `Ctrl+Alt+S`*

### Build Configuration

The project is configured in `platformio.ini` with:
- **Platform**: ESP32 (espressif32)
- **Framework**: Arduino
- **CPU Speed**: 240MHz (maximum performance)
- **Libraries**: Adafruit GFX, SSD1351, BMP3XX

## 🚀 Usage

1. **Power On**: Connect ESP32 to power source (USB or battery)
2. **Normal Operation**: Device shows cycling animations on screen
3. **Throw Detection**: 
   - Throw the device upward
   - System automatically detects motion and tracks altitude
   - Result displays for 7 seconds showing throw height
   - Returns to normal animation cycling

### Display Modes

- **Animation Screens**: Cycles through welcome, play, walk, throw, rocket, and shake animations
- **Result Screen**: Shows measured throw height in cm (< 1m) or meters (≥ 1m)
- **Screen Duration**: Each animation displays for 5 seconds

## 📁 Project Structure

```
snold/
├── src/                     # Source code
│   ├── main.cpp            # Main program logic
│   ├── display.cpp/.h      # Display management
│   ├── sensor.cpp/.h       # Sensor interface
│   ├── images.cpp/.h       # Image rendering functions
│   ├── constants.h         # System constants
│   └── screens/            # Screen management system
├── include/                # Generated image headers
├── assets/                 # Source images and fonts
├── lib/                    # Local libraries
├── platformio.ini          # Build configuration
└── README.md              # This file
```

## ⚙️ Technical Details

### Sensor Configuration
- **Sampling Rate**: 10Hz for smooth throw detection
- **Pressure Oversampling**: 4x for accuracy
- **Temperature Oversampling**: 2x
- **IIR Filter**: Coefficient 3 for noise reduction

### Display Optimization
- **SPI Speed**: 20MHz for fast screen updates
- **DMA Acceleration**: Hardware-accelerated image transfers
- **Multiple Rendering Methods**: Optimized for different image sizes
- **RGB565 Format**: 16-bit color depth for memory efficiency

### Throw Detection Algorithm
1. Monitors vertical velocity (threshold: 2 m/s upward)
2. Tracks maximum altitude during flight
3. Confirms throw end with descent detection
4. Calculates total height difference

## 🔧 Development

### Adding New Images
1. Place PNG files in `assets/img/` or `assets/img_new/`
2. Run image conversion script:
   ```bash
   python3 convert_all_images_rgb565.py
   ```
3. Include generated headers in `main.cpp`

### Customizing Animations
- Modify screen cycling in `main.cpp`
- Adjust `SCREEN_DISPLAY_DURATION` for timing
- Add new screen types in `src/screens/`

## 📚 Dependencies

All dependencies are automatically managed by PlatformIO:

- **Adafruit GFX Library** (^1.11.9): Graphics primitives
- **Adafruit SSD1351 Library** (^1.3.2): OLED display driver  
- **Adafruit BMP3XX Library** (^2.1.6): Barometric sensor interface

## 🎓 Academic Context

This project demonstrates practical application of:
- **Embedded Systems Programming** with ESP32/Arduino framework
- **Sensor Data Processing** and real-time signal analysis
- **Graphics Programming** with optimized rendering techniques
- **State Machine Implementation** for throw detection
- **Hardware Integration** with I2C and SPI protocols

## 📄 License

This project is developed for educational purposes as part of the Applied Computer Science Master's program at HTW Dresden.

---

**HTW Dresden** | **Applied Computer Science** | **Master's Program** 
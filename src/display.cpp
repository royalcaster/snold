#include "display.h"
#include <SPI.h>
#ifdef ESP32
#include <driver/spi_master.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#endif

Adafruit_SSD1351 display = Adafruit_SSD1351(
  OLED_WIDTH, OLED_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);
#ifdef ESP32
static spi_device_handle_t spi_device;
static volatile bool spi_transfer_done = true;
static bool dma_initialized = false;

// DMA callback function
static void IRAM_ATTR spi_post_transfer_callback(spi_transaction_t *t) {
    spi_transfer_done = true;
}
#endif



// Initialize ESP32 native SPI with DMA
void initializeNativeSPIDMA() {
#ifdef ESP32
    if (dma_initialized) return;
    
    Serial.println("=== INITIALIZING ESP32 NATIVE SPI DMA ===");
    
    // Configure SPI bus
    spi_bus_config_t buscfg = {};
    buscfg.miso_io_num = -1;                    // No MISO needed for display
    buscfg.mosi_io_num = 23;                    // MOSI pin (adjust for your wiring)
    buscfg.sclk_io_num = 18;                    // SCLK pin (adjust for your wiring)
    buscfg.quadwp_io_num = -1;                  // Not used
    buscfg.quadhd_io_num = -1;                  // Not used
    buscfg.max_transfer_sz = 128 * 128 * 2;     // Max transfer size for 128x128 RGB565 image
    buscfg.flags = SPICOMMON_BUSFLAG_MASTER;
    
    // Configure SPI device
    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 40000000;           // 40MHz - MAXIMUM for SSD1351 with good wiring
    devcfg.mode = 0;                            // SPI mode 0
    devcfg.spics_io_num = 5;                    // CS pin (adjust for your wiring)
    devcfg.queue_size = 2;                      // Allow 2 queued transactions for ping-pong
    devcfg.pre_cb = nullptr;                    // No pre-callback needed
    devcfg.post_cb = spi_post_transfer_callback; // Post-callback to know when done
    devcfg.flags = SPI_DEVICE_HALFDUPLEX;       // Half-duplex for display
    
    // Initialize SPI bus with DMA
    esp_err_t ret = spi_bus_initialize(VSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        Serial.printf("SPI bus init failed: %s\n", esp_err_to_name(ret));
        return;
    }
    
    // Add device to SPI bus
    ret = spi_bus_add_device(VSPI_HOST, &devcfg, &spi_device);
    if (ret != ESP_OK) {
        Serial.printf("SPI device add failed: %s\n", esp_err_to_name(ret));
        return;
    }
    
    dma_initialized = true;
    Serial.println("ESP32 Native SPI DMA initialized at 40MHz!");
#else
    Serial.println("DMA not available on this platform");
#endif
}

// DMA transfer function
void sendDataDMA(const uint8_t* data, size_t length, bool wait_for_completion) {
#ifdef ESP32
    if (!dma_initialized) {
        Serial.println("DMA not initialized!");
        return;
    }
    
    // Wait for previous transfer to complete
    while (!spi_transfer_done) {
        vTaskDelay(1);
    }
    
    // Prepare transaction
    spi_transaction_t trans = {};
    trans.length = length * 8;                  // Length in bits
    trans.tx_buffer = data;                     // Data to send
    
    // Start DMA transfer
    spi_transfer_done = false;
    esp_err_t ret = spi_device_queue_trans(spi_device, &trans, portMAX_DELAY);
    
    if (ret != ESP_OK) {
        Serial.printf("SPI DMA transfer failed: %s\n", esp_err_to_name(ret));
        spi_transfer_done = true;
        return;
    }
    
    // Optionally wait for completion
    if (wait_for_completion) {
        while (!spi_transfer_done) {
            vTaskDelay(1);
        }
    }
#else
    Serial.println("DMA not available on this platform");
#endif
}

// Send image via DMA
void sendImageDMA(const uint16_t* imageData, int width, int height) {
#ifdef ESP32
    if (!dma_initialized) {
        return;
    }
    
    size_t imageSize = width * height * 2;
    
    sendDataDMA((const uint8_t*)imageData, imageSize, false);
#endif
}

/**
 * @brief Benchmark DMA vs regular SPI performance
 */
void benchmarkDMAPerformance() {
#ifdef ESP32
    const int TEST_SIZE = 128 * 128;
    uint16_t* testImage = (uint16_t*)malloc(TEST_SIZE * sizeof(uint16_t));
    if (!testImage) {
        return;
    }
    
    for (int i = 0; i < TEST_SIZE; i++) {
        testImage[i] = (i % 2) ? 0xF800 : 0x001F;
    }
    
    const int ITERATIONS = 10;
    
    if (dma_initialized) {
        uint32_t startTime = millis();
        for (int i = 0; i < ITERATIONS; i++) {
            sendImageDMA(testImage, 128, 128);
            while (!spi_transfer_done) {
                vTaskDelay(1);
            }
        }
        uint32_t dmaTime = millis() - startTime;
        float dmaFPS = (ITERATIONS * 1000.0) / dmaTime;
        Serial.printf("DMA Performance: %.1fms per frame (%.1f FPS)\n", dmaTime / (float)ITERATIONS, dmaFPS);
    }
    
    uint32_t startTime = millis();
    for (int i = 0; i < ITERATIONS; i++) {
        display.drawRGBBitmap(0, 0, testImage, 128, 128);
    }
    uint32_t regularTime = millis() - startTime;
    float regularFPS = (ITERATIONS * 1000.0) / regularTime;
    Serial.printf("Regular Performance: %.1fms per frame (%.1f FPS)\n", regularTime / (float)ITERATIONS, regularFPS);
    
    free(testImage);
#endif
}

// Initialize OLED display
void initializeDisplay() {
    // First initialize native DMA
    initializeNativeSPIDMA();
    
    SPI.setFrequency(20000000);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    
    display.begin();
    display.setRotation(1);  // Rotate 90 degrees clockwise
    
    display.fillScreen(BLACK);
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
}

// Initialize display with custom SPI speed
void initializeDisplayCustomSpeed(uint32_t spiSpeed) {
    Serial.printf("Initializing display with %dMHz SPI...\n", spiSpeed / 1000000);
    
    SPI.setFrequency(spiSpeed);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    
    display.begin();
    display.setRotation(1);  // Rotate 90 degrees clockwise
    display.fillScreen(BLACK);
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    Serial.printf("Display ready at %dMHz SPI\n", spiSpeed / 1000000);
}

// Benchmark display performance
void benchmarkDisplay() {
    const int TEST_ITERATIONS = 10;
    
    uint32_t startTime = millis();
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        display.fillScreen(i % 2 ? WHITE : BLACK);
    }
    uint32_t endTime = millis();
    float fillScreenTime = (endTime - startTime) / (float)TEST_ITERATIONS;
    Serial.printf("fillScreen(): %.1fms per frame (%.1f FPS)\n", fillScreenTime, 1000.0 / fillScreenTime);
    
    uint16_t testBitmap[128*4];
    for (int i = 0; i < 128*4; i++) {
        testBitmap[i] = 0xF800;
    }
    
    startTime = millis();
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        for (int row = 0; row < 32; row += 4) {
            display.drawRGBBitmap(0, row, testBitmap, 128, 4);
        }
    }
    endTime = millis();
    float bitmapTime = (endTime - startTime) / (float)TEST_ITERATIONS;
    Serial.printf("drawRGBBitmap(): %.1fms per frame (%.1f FPS)\n", bitmapTime, 1000.0 / bitmapTime);
    
    benchmarkDMAPerformance();
    display.fillScreen(BLACK);
}

// Display test message
void displayTest() {
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Display Test");
  display.setTextColor(CYAN);
  display.println("OLED Ready!");
  display.setTextColor(GREEN);
  display.println("BMP388 Ready!");
  
  delay(2000);
  display.fillScreen(BLACK);
}

// Clear display
void clearDisplay() {
  display.fillScreen(BLACK);
}

// Show message on display
void showMessage(const char* message, uint16_t color) {
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(color);
  display.println(message);
}

/**
 * @brief Shows text using custom font at specific coordinates
 */
void showCustomText(const char* text, int x, int y, uint16_t color, uint8_t fontSize) {
  drawCustomText(&display, text, x, y, color, fontSize);
}

/**
 * @brief Shows a message using custom font (centered)
 */
void showCustomMessage(const char* message, uint16_t color, uint8_t fontSize) {
  display.fillScreen(BLACK);
  
  // Calculate text width for centering
  int textWidth = strlen(message) * fontSize;
  int x = (OLED_WIDTH - textWidth) / 2;
  int y = (OLED_HEIGHT - fontSize) / 2;
  
  drawCustomText(&display, message, x, y, color, fontSize);
}

/**
 * @brief Demonstrates the custom fonts
 */
void displayFontDemo() {
  display.fillScreen(BLACK);
  
  // Show 8x8 font
  showCustomText("8x8 FONT", 10, 10, CYAN, FONT_SIZE_8);
  showCustomText("ABCDEFGHIJKLM", 10, 25, WHITE, FONT_SIZE_8);
  showCustomText("NOPQRSTUVWXYZ", 10, 35, WHITE, FONT_SIZE_8);
  showCustomText("0123456789", 10, 45, GREEN, FONT_SIZE_8);
  showCustomText("!@#$%^&*()", 10, 55, YELLOW, FONT_SIZE_8);
  
  delay(3000);
  
  // Show 16x16 font
  display.fillScreen(BLACK);
  showCustomText("16x16 FONT", 5, 10, MAGENTA, FONT_SIZE_16);
  showCustomText("0123456789", 5, 35, RED, FONT_SIZE_16);
  
  delay(3000);
  display.fillScreen(BLACK);
} 
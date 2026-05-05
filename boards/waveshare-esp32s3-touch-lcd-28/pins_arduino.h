#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

// Board: Waveshare ESP32-S3-Touch-LCD-2.8
// MCU:   ESP32-S3 (16 MB Flash, OPI PSRAM)
// Display: ST7789, 2.8" 240x320 IPS, SPI @ 40 MHz
// Touch:   Hynitron CST328 cap-touch, I2C addr 0x5A
// IMU:     QMI8658C, I2C addr 0x6B  (shared bus)
// RTC:     PCF85063,  I2C addr 0x51  (shared bus)
// All I2C devices share I2C0: SDA=1, SCL=3

#define USB_VID 0x303a
#define USB_PID 0x1001

// UART0
static const uint8_t TX = 43;
static const uint8_t RX = 44;

// I2C0 – Touch / IMU / RTC share this bus
#define GROVE_SDA 1
#define GROVE_SCL 3
static const uint8_t SDA = GROVE_SDA;
static const uint8_t SCL = GROVE_SCL;

// General-purpose SPI bus (CC1101, NRF24, etc.)
// Uses ESP32-S3 SPI2 IOMUX native lines
#define SPI_MOSI_PIN 11
#define SPI_MISO_PIN 13
#define SPI_SCK_PIN  12
#define SPI_SS_PIN   10
static const uint8_t SS   = SPI_SS_PIN;
static const uint8_t MOSI = SPI_MOSI_PIN;
static const uint8_t MISO = SPI_MISO_PIN;
static const uint8_t SCK  = SPI_SCK_PIN;

// SD Card – not fitted
#define SDCARD_CS   -1
#define SDCARD_SCK  -1
#define SDCARD_MISO -1
#define SDCARD_MOSI -1

// CC1101 (external module)
#define CC1101_GDO0_PIN  9
#define CC1101_SS_PIN    10
#define CC1101_MOSI_PIN  SPI_MOSI_PIN
#define CC1101_SCK_PIN   SPI_SCK_PIN
#define CC1101_MISO_PIN  SPI_MISO_PIN

// NRF24L01 (external module)
#define NRF24_CE_PIN   9
#define NRF24_SS_PIN   10
#define NRF24_MOSI_PIN SPI_MOSI_PIN
#define NRF24_SCK_PIN  SPI_SCK_PIN
#define NRF24_MISO_PIN SPI_MISO_PIN

// W5500 (external module)
#define W5500_INT_PIN  -1
#define W5500_SS_PIN   -1
#define W5500_MOSI_PIN SPI_MOSI_PIN
#define W5500_SCK_PIN  SPI_SCK_PIN
#define W5500_MISO_PIN SPI_MISO_PIN

// TFT – ST7789 240x320 @ 40 MHz via SPI3 (HSPI)
// USE_HSPI_PORT -> TFT_eSPI uses SPI3; SPI2 (above) stays free for modules
#define USER_SETUP_LOADED 1
#define USE_HSPI_PORT     1
#define ST7789_DRIVER     1
#define TFT_WIDTH         240
#define TFT_HEIGHT        320
#define TFT_INVERSION_ON
#define TFT_BACKLIGHT_ON  HIGH
#define TFT_MOSI  45
#define TFT_SCLK  40
#define TFT_CS    42
#define TFT_DC    41
#define TFT_RST   39
#define TFT_MISO  -1
#define TFT_BL     5
#define TOUCH_CS  -1
#define SMOOTH_FONT         1
#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY 2500000

// Display / UI
#define HAS_SCREEN 1
#define ROTATION   1      // landscape: effective 320x240
#define MINBRIGHT  1
#define BACKLIGHT  TFT_BL

// Touch – Hynitron CST328 on I2C0 (addr 0x5A, INT=4, RST=2)
#define HAS_TOUCH             1
#define HAS_CAPACITIVE_TOUCH  1
#define CST328_I2C_ADDR       0x5A
#define CST328_I2C_SDA        GROVE_SDA
#define CST328_I2C_SCL        GROVE_SCL
#define CST328_INT_PIN        4
#define CST328_RST_PIN        2

// RTC – PCF85063 on I2C0 (lib/RTC/pcf85063_RTC)
#define HAS_RTC
#define HAS_RTC_PCF85063A
#define RTC_SDA GROVE_SDA
#define RTC_SCL GROVE_SCL

// Font sizes
#define FP 1
#define FM 2
#define FG 3

// Boot / navigation button
#define HAS_BTN   1
#define BTN_ALIAS '"Boot"'
#define BTN_PIN   0
#define BTN_ACT   LOW

// IR (no dedicated pin; configure via Bruce settings)
#define TXLED   -1
#define LED_ON  HIGH
#define LED_OFF LOW

// Serial / GPS
#define SERIAL_TX     43
#define SERIAL_RX     44
#define GPS_SERIAL_TX SERIAL_TX
#define GPS_SERIAL_RX SERIAL_RX

// BadUSB via native USB
#define USB_as_HID 1
#define BAD_TX     GROVE_SDA
#define BAD_RX     GROVE_SCL

// Deep sleep
#define DEEPSLEEP_WAKEUP_PIN BTN_PIN
#define DEEPSLEEP_PIN_ACT    LOW

#endif /* Pins_Arduino_h */

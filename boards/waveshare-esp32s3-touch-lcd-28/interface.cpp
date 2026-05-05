#ifdef WAVESHARE_ESP32S3_TOUCH_LCD_28
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/powerSave.h"

// ---------------------------------------------------------------------------
// CST328 Capacitive touch – raw I2C driver
// Register protocol: 16-bit address, little-endian read
// Touch reg: 0xD0 (hi), 0x00 (lo)  – FT6x36-compatible packet layout
// ---------------------------------------------------------------------------
#define CST328_ADDR CST328_I2C_ADDR  // 0x5A

static bool _cst328_ready = false;

static void _cst328_reset() {
    if (CST328_RST_PIN >= 0) {
        pinMode(CST328_RST_PIN, OUTPUT);
        digitalWrite(CST328_RST_PIN, LOW);
        delay(20);
        digitalWrite(CST328_RST_PIN, HIGH);
        delay(50);
    }
}

static bool _cst328_init() {
    _cst328_reset();
    // probe: write register address and see if ACK received
    Wire.beginTransmission(CST328_ADDR);
    Wire.write(0xD0);
    Wire.write(0x00);
    return (Wire.endTransmission() == 0);
}

// Read one touch point.  Returns true when finger is down.
// Packet from 0xD000:
//   byte 0:  GestureID
//   byte 1:  Finger count (lower nibble)
//   byte 2:  Xh (event | X[11:8])   <- FT6x36 order
//   byte 3:  Xl (X[7:0])
//   byte 4:  Yh (ID   | Y[11:8])
//   byte 5:  Yl (Y[7:0])
//   byte 6:  Pressure / area
static bool _cst328_read(uint16_t &rx, uint16_t &ry) {
    Wire.beginTransmission(CST328_ADDR);
    Wire.write(0xD0);
    Wire.write(0x00);
    if (Wire.endTransmission(false) != 0) return false;

    if (Wire.requestFrom((uint8_t)CST328_ADDR, (uint8_t)7) != 7) return false;

    uint8_t buf[7];
    for (int i = 0; i < 7; i++) buf[i] = Wire.read();

    uint8_t fingers = buf[1] & 0x0F;
    if (fingers == 0) return false;

    rx = ((uint16_t)(buf[2] & 0x0F) << 8) | buf[3];
    ry = ((uint16_t)(buf[4] & 0x0F) << 8) | buf[5];
    return true;
}

// ---------------------------------------------------------------------------
// Bruce HAL interface
// ---------------------------------------------------------------------------
void _setup_gpio() {
    // I2C0 – initialise for Touch, IMU, RTC
    Wire.begin(CST328_I2C_SDA, CST328_I2C_SCL);
    Wire.setClock(400000);

    // Touch sensor init
    _cst328_ready = _cst328_init();
    if (!_cst328_ready) {
        Serial.println("[CST328] init failed – touch disabled");
    }

    // Backlight pin
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
}

void _set_Brightness(uint8_t brightness) {
    static uint8_t _cur = 255;
    if (brightness == _cur) return;
    _cur = brightness;

    if (brightness == 0) {
        digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);
    } else {
        analogWrite(TFT_BL, brightness);
    }
}

void _sleep() {
    analogWrite(TFT_BL, 0);
    delay(5);
    digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);
}

void _wakeup() {
    _set_Brightness(currentScreenBrightness);  // global brightness var from globals
}

// ---------------------------------------------------------------------------
// Touch input
// Coordinate mapping: CST328 reports in native 240x320 (portrait) space.
// Bruce UI runs at effective 320x240 (landscape, ROTATION=1).
// Transform for rotation 1 (90° CW):  screen_x = raw_y ; screen_y = W - 1 - raw_x
// where W = TFT_WIDTH = 240 (native portrait width).
// ---------------------------------------------------------------------------
bool getTouch(uint16_t *x, uint16_t *y) {
    if (!_cst328_ready) return false;

    uint16_t rx, ry;
    if (!_cst328_read(rx, ry)) return false;

    // Clamp to native panel dimensions
    if (rx >= TFT_WIDTH)  rx = TFT_WIDTH  - 1;
    if (ry >= TFT_HEIGHT) ry = TFT_HEIGHT - 1;

#if ROTATION == 0
    *x = rx;
    *y = ry;
#elif ROTATION == 1
    *x = ry;
    *y = (TFT_WIDTH - 1) - rx;
#elif ROTATION == 2
    *x = (TFT_WIDTH  - 1) - rx;
    *y = (TFT_HEIGHT - 1) - ry;
#elif ROTATION == 3
    *x = (TFT_HEIGHT - 1) - ry;
    *y = rx;
#else
    *x = rx;
    *y = ry;
#endif
    return true;
}

// ---------------------------------------------------------------------------
// Input Handler – processes touch and button input
// ---------------------------------------------------------------------------
void InputHandler(void) {
    static long tm = 0;

    // Debounce touch input at 200ms intervals
    if (millis() - tm > 200) {
        uint16_t x, y;
        if (getTouch(&x, &y)) {
            tm = millis();

            // Wake up screen if sleeping
            if (!wakeUpScreen()) {
                AnyKeyPress = true;
            } else {
                return;
            }

            // Update global touch point
            touchPoint.x = x;
            touchPoint.y = y;
            touchPoint.pressed = true;
        }
    }
}

#endif // WAVESHARE_ESP32S3_TOUCH_LCD_28

#include "Mode.h"
#include <Arduino.h>  // Dla funkcji constrain i map

namespace am {

// ---------- TEMP ----------

TempMode::TempMode() : kelvin(3500) {}

void TempMode::adjust(int value) {
    kelvin = constrain(kelvin + value * 100, 2900, 5600);
}

void TempMode::execute(Adafruit_NeoPixel& strip) {
    int r, g, b;

    if (kelvin <= 4000) {
        r = 255;
        g = map(kelvin, 2900, 4000, 50, 165);
        b = map(kelvin, 2900, 4000, 0, 50);
    } else {
        r = map(kelvin, 4000, 5600, 255, 200);
        g = 255;
        b = map(kelvin, 4000, 5600, 50, 200);
    }

    uint32_t color = strip.Color(r, g, b);

    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

const char* TempMode::getName() const {
    return "TEMP";
}

const char* TempMode::getDisplayValue() const {
    static char valueText[10];
    snprintf(valueText, sizeof(valueText), "%dK", kelvin);
    return valueText;
}

// ---------- HSI ----------

HueMode::HueMode() : hue(180) {}

void HueMode::adjust(int value) {
    hue = constrain(hue + value, 0, 255);
}

void HueMode::execute(Adafruit_NeoPixel& strip) {
    uint32_t color = strip.ColorHSV(hue * 182, 255, 255);

    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

const char* HueMode::getName() const {
    return "HUE";
}

const char* HueMode::getDisplayValue() const {
    static char valueText[10];
    snprintf(valueText, sizeof(valueText), "%d", hue);
    return valueText;
}

// ---------- POWER ----------

PowerMode::PowerMode() : powerPercentage(35) {}

void PowerMode::adjust(int value) {
    powerPercentage = constrain(powerPercentage + value, 0, 100);
}

void PowerMode::execute(Adafruit_NeoPixel& strip) {
    strip.setBrightness(map(powerPercentage, 0, 100, 0, 255));
    strip.show();
}

const char* PowerMode::getName() const {
    return "POWER";
}

const char* PowerMode::getDisplayValue() const {
    static char valueText[10];
    snprintf(valueText, sizeof(valueText), "%d%%", powerPercentage);
    return valueText;
}

}

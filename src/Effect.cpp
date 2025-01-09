#include "Effect.h"
#include <Arduino.h>

namespace am {

// Efekt "Police Car"
void PoliceCarEffect::run(Adafruit_NeoPixel& strip) {
    static unsigned long lastUpdate = 0;
    static int phase = 0;
    static int brightness = 0;
    static bool increasing = true;

    const int interval = 20;
    const int transitionTime = 500;
    unsigned long now = millis();

    if (now - lastUpdate >= interval) {
        lastUpdate = now;

        if (increasing) {
            brightness += 255 * interval / transitionTime;
            if (brightness >= 255) {
                brightness = 255;
                increasing = false;
            }
        } else {
            brightness -= 255 * interval / transitionTime;
            if (brightness <= 0) {
                brightness = 0;
                increasing = true;
                phase = (phase + 1) % 4;
            }
        }

        uint32_t color = (phase < 2) ? strip.Color(brightness, 0, 0) : strip.Color(0, 0, brightness);

        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, color);
        }
        strip.show();
    }
}

// Efekt "TV"
void TVEffect::run(Adafruit_NeoPixel& strip) {
    static unsigned long lastUpdate = 0;
    static unsigned long fadeTime = 500;
    static int brightness = 0;
    static int targetBrightness = 0;
    static uint32_t currentColor = 0;

    unsigned long now = millis();

    if (now - lastUpdate >= fadeTime) {
        lastUpdate = now;
        fadeTime = random(100, 500);
        targetBrightness = random(50, 200);
        uint8_t r = random(50, 150);
        uint8_t g = random(50, 200);
        uint8_t b = random(100, 255);
        currentColor = strip.Color(r, g, b);
    }

    brightness += (brightness < targetBrightness) ? 10 : -10;
    brightness = constrain(brightness, 0, targetBrightness);

    for (int i = 0; i < strip.numPixels(); i++) {
        uint8_t r = ((currentColor >> 16) & 0xFF) * brightness / 255;
        uint8_t g = ((currentColor >> 8) & 0xFF) * brightness / 255;
        uint8_t b = (currentColor & 0xFF) * brightness / 255;
        strip.setPixelColor(i, strip.Color(r, g, b));
    }
    strip.show();
}

// Efekt "Paparazzi"
void PaparazziEffect::run(Adafruit_NeoPixel& strip) {
    static unsigned long lastUpdate = 0;
    static unsigned long currentDuration = 0;
    static int flashCount = 0;
    static bool isFlashOn = false;

    unsigned long now = millis();

    if (now - lastUpdate >= currentDuration) {
        lastUpdate = now;

        if (flashCount == 0) {
            flashCount = random(3, 7);
            currentDuration = random(1000, 3000);
            isFlashOn = false;
            strip.clear();
        } else {
            if (isFlashOn) {
                strip.clear();
                currentDuration = random(50, 150);
                isFlashOn = false;
            } else {
                uint32_t color = strip.Color(255, 255, 255);
                for (int i = 0; i < strip.numPixels(); i++) {
                    strip.setPixelColor(i, color);
                }
                currentDuration = random(30, 100);
                isFlashOn = true;
                flashCount--;
            }
        }
        strip.show();
    }
}

// Efekt "Fire"
void FireEffect::run(Adafruit_NeoPixel& strip) {
    static unsigned long lastUpdate = 0;
    const int updateInterval = 50;

    if (millis() - lastUpdate >= updateInterval) {
        lastUpdate = millis();

        for (int i = 0; i < strip.numPixels(); i++) {
            uint8_t brightness = random(50, 255);
            uint8_t r = brightness;
            uint8_t g = random(brightness / 4, brightness / 2);
            uint8_t b = random(0, brightness / 8);
            if (random(0, 10) > 7) {
                r /= 2; g /= 2; b /= 2;
            }
            strip.setPixelColor(i, strip.Color(r, g, b));
        }
        strip.show();
    }
}

// Efekt "Disco"
void DiscoEffect::run(Adafruit_NeoPixel& strip) {
    static unsigned long lastUpdate = 0;
    const int updateInterval = 150;

    const uint32_t colors[] = {
        strip.Color(255, 0, 0),
        strip.Color(0, 255, 0),
        strip.Color(0, 0, 255),
        strip.Color(255, 255, 0),
        strip.Color(255, 0, 255),
        strip.Color(255, 165, 0)
    };

    if (millis() - lastUpdate >= updateInterval) {
        lastUpdate = millis();
        uint32_t color = colors[random(0, sizeof(colors) / sizeof(colors[0]))];
        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, color);
        }
        strip.show();
    }
}

// Efekt "Firework"
void FireworkEffect::run(Adafruit_NeoPixel& strip) {
    static unsigned long lastUpdate = 0;
    static unsigned long delayBetweenBursts = 1000;
    static uint32_t currentColor = 0;
    static int brightness = 0;
    static bool isFading = false;
    static bool waitingAfterFade = false;

    const int fadeStep = 10;
    const int updateInterval = 50;
    unsigned long now = millis();

    if (now - lastUpdate >= updateInterval) {
        lastUpdate = now;

        if (waitingAfterFade) {
            static unsigned long waitStart = 0;
            if (waitStart == 0) waitStart = now;
            if (now - waitStart >= delayBetweenBursts) {
                waitingAfterFade = false;
                waitStart = 0;
            }
            return;
        }

        if (!isFading && brightness == 0) {
            int dominant = random(0, 3);
            uint8_t r = (dominant == 0) ? random(240, 255) : random(10, 75);
            uint8_t g = (dominant == 1) ? random(240, 255) : random(10, 75);
            uint8_t b = (dominant == 2) ? random(240, 255) : random(10, 75);
            currentColor = strip.Color(r, g, b);
            brightness = 255;
            isFading = true;
        }

        if (isFading) {
            brightness -= fadeStep;
            if (brightness <= 0) {
                brightness = 0;
                isFading = false;
                waitingAfterFade = true;
                delayBetweenBursts = random(1000, 3000);
                strip.clear();
                strip.show();
                return;
            }
        }

        uint8_t r = ((currentColor >> 16) & 0xFF) * brightness / 255;
        uint8_t g = ((currentColor >> 8) & 0xFF) * brightness / 255;
        uint8_t b = (currentColor & 0xFF) * brightness / 255;

        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(r, g, b));
        }
        strip.show();
    }
}

}

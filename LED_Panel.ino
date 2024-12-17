#include <Adafruit_NeoPixel.h>

// LED setup
#define LED_PIN     6
#define NUM_LEDS    48
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Ustawienie pełnej jasności na kolor czerwony (można zmienić na inny)
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255)); // Czerwony kolor
  }
  strip.setBrightness(255); // Ustawienie maksymalnej jasności
  strip.show(); // Zaktualizowanie stanu diod
}

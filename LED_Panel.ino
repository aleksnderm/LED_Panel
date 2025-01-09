#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "src/Mode.h"
#include "src/EffectMode.h"

// Ustawienia i zmienne globalne
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_I2C_ADDRESS 0x3C
#define LED_PIN 6
#define NUM_LEDS 48

// Deklaracja funkcji
void updateLEDs();
void drawUI();
void updateEncoder();
void handleButton();

// Ustawienia wyświetlacza OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_I2C_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Czcionki
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

// Ustawienia macierzy LED
#define LED_PIN 6
#define NUM_LEDS 48
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Piny enkodera obrotowego
#define PIN_A 2
#define PIN_B 3
#define PIN_SW 4

// Zmienne enkodera
volatile int encoderPosition = 0;
volatile bool encoderMoved = false;
volatile bool buttonPressed = false;

// Tryby
am::Mode* modes[4]; // Tablica wskaźników do trybów
int currentModeIndex = 3; // Indeks aktualnego trybu
bool valueSelected = true;

// Funkcja ustawiająca jasność i kolor LED
void updateLEDs() {
  modes[currentModeIndex]->execute(strip); // Wywołanie aktualnego trybu
}

// Czułość enkodera
const int ENCODER_SENSITIVITY = 4;
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;

// Zmienne do UI
int batteryLevel = 85;  // Poziom baterii (0-100)
bool bluetoothConnected = true;  // Status Bluetooth (połączony: true / niepołączony: false)

// Funkcja rysująca wartość trybu
void drawValue(int x, int y, am::Mode* mode, bool highlight) {
  display.setFont(&FreeMono12pt7b);
  display.setTextSize(1);
  const char* valueText = mode->getDisplayValue(); // Pobranie wartości trybu

  if (highlight) {
    int16_t x1, y1;
    uint16_t w, h;
    display.setFont(&FreeMonoBold12pt7b);
    display.getTextBounds(valueText, x, y, &x1, &y1, &w, &h);
    display.fillRect(x1 - 1, y1 - 1, w + 2, h + 2, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(x, y);
    display.print(valueText);
    display.setTextColor(SSD1306_WHITE);
  } else {
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.print(valueText);
  }
}

// Funkcja rysująca nazwę trybu
void drawMode(int x, int y, am::Mode* mode, bool highlight) {
  display.setFont(&FreeMono9pt7b);
  display.setTextSize(1);

  const char* modeText = mode->getName(); // Pobranie nazwy trybu

  if (highlight) {
    int16_t x1, y1;
    uint16_t w, h;
    display.setFont(&FreeMonoBold9pt7b);
    display.getTextBounds(modeText, x, y, &x1, &y1, &w, &h);
    display.fillRect(x1 - 1, y1 - 1, w + 2, h + 2, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(x, y);
    display.print(modeText);
    display.setTextColor(SSD1306_WHITE);
  } else {
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.print(modeText);
  }
}

// Funkcja rysująca baterię
void drawBattery(int x, int y, int level) {
  display.drawRect(x, y, 9, 13, SSD1306_WHITE);
  display.fillRect(x + 2, y - 3, 5, 2, SSD1306_WHITE);
  int fillWidth = map(level, 0, 100, 1, 11);
  display.fillRect(x + 2, y + 2, 5, fillWidth, SSD1306_WHITE);
}

// Funkcja rysująca Bluetooth
void drawBluetooth(int x, int y) {
  display.setFont(&FreeMono9pt7b);
  display.setTextSize(0.5);
  display.setCursor(x, y);
  display.print("BT");
}

// Funkcja rysująca UI
void drawUI() {
  display.clearDisplay();
  drawMode(0, 11, modes[currentModeIndex], !valueSelected);
  drawValue(0, 30, modes[currentModeIndex], valueSelected);
  drawBattery(SCREEN_WIDTH - 9, 19, batteryLevel);
  drawBluetooth(SCREEN_WIDTH - 21, 11);
  display.display();
}

// Funkcja aktualizująca enkoder
void updateEncoder() {
  int MSB = digitalRead(PIN_A);
  int LSB = digitalRead(PIN_B);
  int encoded = (MSB << 1) | LSB;
  static int lastEncoded = 0;
  static const int8_t encoderTable[16] = {
    0, -1,  1,  0,
    1,  0,  0, -1,
   -1,  0,  0,  1,
    0,  1, -1,  0
  };
  encoderPosition += encoderTable[((lastEncoded << 2) | encoded) & 0x0F];
  encoderMoved = true;
  lastEncoded = encoded;
}

// Funkcja obsługująca przycisk
void handleButton() {
  unsigned long currentTime = millis();
  if ((currentTime - lastButtonPress) > debounceDelay) {
    if (digitalRead(PIN_SW) == LOW) {
      buttonPressed = true;
      lastButtonPress = currentTime;
    }
  }
}

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    for (;;);
  }
  display.clearDisplay();

  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_SW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_A), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_B), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_SW), handleButton, FALLING);

  strip.begin();
  strip.show();

  // Inicjalizacja trybów
  modes[0] = new am::TempMode();
  modes[1] = new am::HueMode();
  modes[2] = new am::PowerMode();
  modes[3] = new am::EffectsMode();

  updateLEDs(); // Ustawienie LED na start
  drawUI();     // Wyświetlenie interfejsu na start
}





void loop() {
  // Obsługa przycisku - zmiana trybu
  if (buttonPressed) {
    buttonPressed = false;
    currentModeIndex = (currentModeIndex + 1) % 4; // Zmiana trybu (4 tryby: Temp, Hue, Power, Effects)
    drawUI();
  }

  // Obsługa enkodera - dostosowanie wartości w bieżącym trybie
  if (encoderMoved) {
    encoderMoved = false;
    int effectiveChange = encoderPosition / ENCODER_SENSITIVITY;
    encoderPosition %= ENCODER_SENSITIVITY;

    modes[currentModeIndex]->adjust(effectiveChange); // Dostosowanie wartości trybu
    drawUI();
  }

  // Obsługa trybu efektów (EffectsMode)
  if (currentModeIndex == 3) { // Indeks 3 to EffectsMode
    modes[currentModeIndex]->execute(strip); // Wywołanie pętli efektów
  } else {
    // Jednorazowe wykonanie dla pozostałych trybów
    modes[currentModeIndex]->execute(strip);
  }
}


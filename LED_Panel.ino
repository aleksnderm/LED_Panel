#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Ustawienia wyświetlacza OLED
#define SCREEN_WIDTH 128 // Szerokość OLED w pikselach
#define SCREEN_HEIGHT 32 // Wysokość OLED w pikselach
#define OLED_I2C_ADDRESS 0x3C

// Inicjalizacja wyświetlacza OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Czcionki
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

// Piny enkodera obrotowego
#define PIN_A 2
#define PIN_B 3
#define PIN_SW 4

// Zmienne enkodera
volatile int encoderPosition = 0;
volatile bool encoderMoved = false;
volatile bool buttonPressed = false;

// Tryby
enum Mode { TEMP, POWER, HUE }; // Dostępne tryby
Mode currentMode = TEMP;        // Aktualny tryb
bool valueSelected = true;     // Czy zmieniamy wartość

// Zmienne wartości
int kelvin = 3400;              // Temperatura barwowa (2900-5600)
int powerPercentage = 35;       // Moc LED (0-100%)
int hue = 180;                  // Odcień (0-360)

// Czułość enkodera
const int ENCODER_SENSITIVITY = 4; // Zmiana wartości co 4 kroki enkodera

// Debouncing przycisku
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200; // 200 ms

// Funkcja rysująca wartość trybu
void drawValue(int x, int y, Mode mode, bool highlight) {
  display.setFont(&FreeMono12pt7b);
  display.setTextSize(1);

  char valueText[10];

  // Formatowanie wartości zależnie od trybu
  switch (mode) {
    case TEMP:
      sprintf(valueText, "%dK", kelvin);
      break;
    case POWER:
      sprintf(valueText, "%d%%", powerPercentage);
      break;
    case HUE:
      sprintf(valueText, "%d°", hue);
      break;
  }

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
void drawMode(int x, int y, Mode mode, bool highlight) {
  display.setFont(&FreeMono9pt7b);
  display.setTextSize(1);

  const char *modeText;
  switch (mode) {
    case TEMP:
      modeText = "TEMP";
      break;
    case POWER:
      modeText = "POWER";
      break;
    case HUE:
      modeText = "HUE";
      break;
  }

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

// Funkcja do odświeżania UI
void drawUI() {
  display.clearDisplay();

  // Rysowanie aktywnego trybu i wartości
  drawMode(0, 11, currentMode, !valueSelected);
  drawValue(0, 30, currentMode, valueSelected);

  display.display();
}

// Obsługa enkodera obrotowego
void updateEncoder() {
  int MSB = digitalRead(PIN_A);
  int LSB = digitalRead(PIN_B);
  int encoded = (MSB << 1) | LSB;
  static int lastEncoded = 0;

  int sum = (lastEncoded << 2) | encoded;
  static const int8_t encoderTable[16] = {
    0, -1,  1,  0,
    1,  0,  0, -1,
   -1,  0,  0,  1,
    0,  1, -1,  0
  };

  encoderPosition += encoderTable[sum & 0x0F];
  encoderMoved = true;
  lastEncoded = encoded;
}

// Obsługa przycisku enkodera
void handleButton() {
  unsigned long currentTime = millis();

  // Sprawdzanie debounce
  if ((currentTime - lastButtonPress) > debounceDelay) {
    buttonPressed = true;
    lastButtonPress = currentTime;
  }
}

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("OLED initialization failed!");
    for (;;);
  }
  display.clearDisplay();

  // Piny enkodera
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_SW, INPUT_PULLUP);

  // Przerwania
  attachInterrupt(digitalPinToInterrupt(PIN_A), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_B), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_SW), handleButton, FALLING);

  drawUI();
}

void loop() {
  if (buttonPressed) {
    buttonPressed = false;
    valueSelected = !valueSelected; // Przełączanie między trybem a wartością
    drawUI();
  }

  if (encoderMoved) {
    encoderMoved = false;

    int effectiveChange = encoderPosition / ENCODER_SENSITIVITY; // Użyj mniejszej czułości
    encoderPosition %= ENCODER_SENSITIVITY; // Resetuj nadmiar

    if (valueSelected) { // Zmiana wartości w danym trybie
      if (currentMode == TEMP) {
        kelvin += effectiveChange * 100;
        if (kelvin > 5600) kelvin = 5600;
        if (kelvin < 2900) kelvin = 2900;
      } else if (currentMode == POWER) {
        powerPercentage += effectiveChange * 5;
        if (powerPercentage > 100) powerPercentage = 100;
        if (powerPercentage < 0) powerPercentage = 0;
      } else if (currentMode == HUE) {
        hue += effectiveChange * 10;
        if (hue > 360) hue = 360;
        if (hue < 0) hue = 0;
      }
    } else { // Przełączanie trybu
      currentMode = static_cast<Mode>((currentMode + effectiveChange + 3) % 3);
    }

    drawUI();
  }
}
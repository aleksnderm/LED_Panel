#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Ustawienia wyświetlacza OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Piny enkodera
#define PIN_A 2    // Enkoder - pin A
#define PIN_B 3    // Enkoder - pin B
#define PIN_SW 4   // Enkoder - przycisk
#define DEBOUNCE_TIME 5  // Czas debouncingu w milisekundach

// Zmienne enkodera
volatile int position = 0;   // Aktualna pozycja enkodera
volatile bool moved = false; // Flaga zmiany pozycji
int kelvin = 3400;           // Temperatura barwowa w Kelvinach

// Odczyt aktualnego stanu enkodera (dla kwadraturowego odczytu)
int lastEncoded = 0; // Poprzedni stan enkodera (A i B razem)
int lastKelvin = 0;

// Tablica kwadraturowa do określenia kierunku obrotu
const int8_t encoderTable[16] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
 -1,  0,  0,  1,
  0,  1, -1,  0
};

// Funkcja obsługi enkodera
void updateEncoder() {
  int MSB = digitalRead(PIN_A); // Najbardziej znaczący bit
  int LSB = digitalRead(PIN_B); // Najmniej znaczący bit

  int encoded = (MSB << 1) | LSB; // Połączenie A i B w jeden numer
  int sum = (lastEncoded << 2) | encoded; // Przesunięcie i zapis do historii
  int direction = encoderTable[sum & 0x0F]; // Wyznaczenie kierunku z tablicy

  if (direction != 0) {
    position += direction; // Zwiększenie lub zmniejszenie pozycji
    moved = true;          // Flaga zmiany
  }

  lastEncoded = encoded; // Zapisanie stanu jako poprzedni
}

// Inicjalizacja
void setup() {
  // Inicjalizacja wyświetlacza OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("OLED initialization failed!");
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Encoder Test");
  display.display();
  delay(2000);

  // Ustawienie pinów enkodera
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_SW, INPUT_PULLUP);

  // Przerwania na zmianę stanu pinu A i B
  attachInterrupt(digitalPinToInterrupt(PIN_A), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_B), updateEncoder, CHANGE);
}

void loop() {
  if (moved) { // Jeśli nastąpił ruch enkodera
    moved = false;

    // Aktualizacja temperatury na podstawie pozycji
    kelvin = 3400 + (position * 100);
    if (kelvin > 6500) kelvin = 6500;
    if (kelvin < 2500) kelvin = 2500;

    // Wyświetlenie na OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Position: ");
    display.print(position);
    display.setCursor(0, 16);
    display.print("Kelvin: ");
    display.print(kelvin);
    display.display();
  }
}

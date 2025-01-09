#ifndef MODE_H
#define MODE_H

#include <Adafruit_NeoPixel.h>

namespace am {

/**
 * @brief Bazowa klasa dla różnych trybów świecenia LED.
 */
class Mode {
public:
    virtual ~Mode() {}
    virtual void adjust(int value) = 0;                // Dostosowanie wartości trybu
    virtual void execute(Adafruit_NeoPixel& strip) = 0; // Wykonanie trybu
    virtual const char* getName() const = 0;          // Nazwa trybu
    virtual const char* getDisplayValue() const = 0;  // Aktualna wartość trybu
};

/**
 * @brief Tryb zmieniający temperaturę barwową LED.
 */
class TempMode : public Mode {
public:
    TempMode();
    void adjust(int value) override;
    void execute(Adafruit_NeoPixel& strip) override;
    const char* getName() const override;
    const char* getDisplayValue() const override;

private:
    int kelvin;
};

/**
 * @brief Tryb zmieniający kolor LED w przestrzeni HUE.
 */
class HueMode : public Mode {
public:
    HueMode();
    void adjust(int value) override;
    void execute(Adafruit_NeoPixel& strip) override;
    const char* getName() const override;
    const char* getDisplayValue() const override;

private:
    int hue;
};

/**
 * @brief Tryb zmieniający moc/jasność LED.
 */
class PowerMode : public Mode {
public:
    PowerMode();
    void adjust(int value) override;
    void execute(Adafruit_NeoPixel& strip) override;
    const char* getName() const override;
    const char* getDisplayValue() const override;

private:
    int powerPercentage;
};

} // namespace am

#endif // MODE_H

#ifndef EFFECT_H
#define EFFECT_H

#include <Adafruit_NeoPixel.h>

namespace am {

class Effect {
public:
    virtual ~Effect() {}
    virtual void run(Adafruit_NeoPixel& strip) = 0;
    virtual const char* getName() const = 0;
    virtual void stop(Adafruit_NeoPixel& strip) { 
        strip.clear(); 
        strip.show(); 
    }
};

// Efekt światła radiowozu
class PoliceCarEffect : public Effect {
public:
    const char* getName() const override { return "Police"; }
    void run(Adafruit_NeoPixel& strip) override;
};

// Efekt światła telewizora
class TVEffect : public Effect {
public:
    const char* getName() const override { return "TV"; }
    void run(Adafruit_NeoPixel& strip) override;
};

// Efekt błysków paparazzi
class PaparazziEffect : public Effect {
public:
    const char* getName() const override { return "Paparazzi"; }
    void run(Adafruit_NeoPixel& strip) override;
};

// Efekt płomienia ognia
class FireEffect : public Effect {
public:
    const char* getName() const override { return "Fire"; }
    void run(Adafruit_NeoPixel& strip) override;
};

// Efekt disco
class DiscoEffect : public Effect {
public:
    const char* getName() const override { return "Disco"; }
    void run(Adafruit_NeoPixel& strip) override;
};

// Efekt fajerwerków
class FireworkEffect : public Effect {
public:
    const char* getName() const override { return "Firework"; }
    void run(Adafruit_NeoPixel& strip) override;
};

}

#endif

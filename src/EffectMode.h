#ifndef EFFECTS_MODE_H
#define EFFECTS_MODE_H

#include "Mode.h"
#include "Effect.h"
#include <Adafruit_NeoPixel.h>

namespace am {

/**
 * @brief Tryb zawierający listę efektów świetlnych.
 */
class EffectsMode : public Mode {
public:
    EffectsMode();
    ~EffectsMode();

    void adjust(int value) override;
    void execute(Adafruit_NeoPixel& strip) override;
    const char* getName() const override { return "Effects"; }
    const char* getDisplayValue() const override;

private:
    static const int MAX_EFFECTS = 10;
    Effect* effects[MAX_EFFECTS];
    int effectCount;
    int currentEffectIndex;
};

}

#endif

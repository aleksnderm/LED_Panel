#include "EffectMode.h"
#include "Effect.h"

namespace am {

EffectsMode::EffectsMode() : currentEffectIndex(0), effectCount(0) {
    effects[effectCount++] = new PoliceCarEffect();
    effects[effectCount++] = new TVEffect();
    effects[effectCount++] = new PaparazziEffect();
    effects[effectCount++] = new FireEffect();
    effects[effectCount++] = new DiscoEffect();
    effects[effectCount++] = new FireworkEffect();
    // Tutaj można dodać kolejne efekty
}

EffectsMode::~EffectsMode() {
    for (int i = 0; i < effectCount; i++) {
        delete effects[i];
    }
}

void EffectsMode::adjust(int value) {
    currentEffectIndex = (currentEffectIndex + value + effectCount) % effectCount;
}

void EffectsMode::execute(Adafruit_NeoPixel& strip) {
    effects[currentEffectIndex]->run(strip);
}

const char* EffectsMode::getDisplayValue() const {
    return effects[currentEffectIndex]->getName();
}

}

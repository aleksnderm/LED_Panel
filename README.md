# LED Panel Controller

## Project Description
LED Panel Controller is a C++ application designed to control an LED panel using a rotary encoder. The project was created to simulate various lighting effects, such as a police car, fire, fireworks, TV, paparazzi flashes, and disco mode. The user interface is implemented on an SSD1306 OLED display.

## Project Features

### Lighting Modes
- **Color Temperature**: Adjust the warmth of the light (2900K-5600K).
- **Color (Hue)**: Set the color in the HSV color space.
- **Power**: Adjust the brightness of the LEDs.
- **Effects**: Choose from various lighting animations.

### Lighting Effects
- **Police Car**: Simulates police sirens.
- **TV**: Mimics the flickering light of a television.
- **Paparazzi**: Flashes simulating camera flashes.
- **Fire**: Simulates a flickering fire effect.
- **Disco**: Multicolor flashing for a disco ambiance.
- **Fireworks**: Simulates fireworks lighting effects.


https://github.com/user-attachments/assets/5374729a-7cb9-46d5-9871-32689dc11b7f


### User Interface
- Displays the current mode and its values on the OLED screen.
- Uses a rotary encoder to change values and modes.

## Technologies
- **C++**: Programming language used for implementation.
- **Adafruit NeoPixel**: Library for controlling the LED strip.
- **Adafruit SSD1306**: Library for handling the OLED display.
- **Arduino**: Microcontroller platform used to run the project.

## Project Structure
```
LED-Panel-Controller/
  ├── src/
    ├── Mode.h 
    ├── Mode.cpp
    ├── Effect.h
    ├── Effect.cpp
    ├── EffectMode.h
    ├── EffectMode.cpp
  ├── Panel_ledowy.ino
  ├── README.md
  ├── Documentation
```

## Hardware Requirements
- Arduino-compatible microcontroller (e.g., Arduino Uno, Mega).
- LED strip compatible with Adafruit NeoPixel (WS2812B or similar).
- SSD1306 OLED display.
- Rotary encoder with a button.

## Installation
1. Clone the project repository:
   ```bash
   git clone https://github.com/YourAccount/LED-Panel-Controller.git
   cd LED-Panel-Controller
   

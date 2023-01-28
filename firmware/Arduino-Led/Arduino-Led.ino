//#define debug

// user variables
uint8_t mode = 0;         // Current mode (start mode)
uint8_t brightness = 80;  // Current brightness
uint8_t scale = 1;        // Scale, for some modes

// system variables
uint8_t code;         // Code of button
uint8_t counter = 0;  // Iteration counter, for some modes
uint8_t modes = 6;    // number of modes
int8_t dir = 1;

uint8_t r = 255;
uint8_t g = 255;
uint8_t b = 255;

uint8_t h = 0;
uint8_t s = 0;
uint8_t v = 255;

#ifdef debug
#define LED_PIN 4
#define IR_PIN 3
#define LED_NUM 5
#else
#define LED_PIN 2    // Use for final firmware
#define IR_PIN 14    // Use for final firmware
#define LED_NUM 300  // Use for final firmware
#endif

// Connecting library
#include <FastLED.h>
CRGB leds[LED_NUM];
#include <IRremote.hpp>
#include "buttons.h"

// Using NEC remote controller
#define DECODE_NEC

void setup() {
#ifdef debug
  Serial.begin(9600);   // Initialization Serial for debug
  pinMode(9, OUTPUT);   // Red pin for LED
  pinMode(10, OUTPUT);  // Blue pin for LED
  pinMode(5, OUTPUT);   // Green pin for LED
#endif
  IrReceiver.begin(IR_PIN);                                                             // Setup pin for IR receiver
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM).setCorrection(TypicalLEDStrip);  // Create strip
  FastLED.setBrightness(brightness);                                                    // Set default brightness
}

void decodeIR() {
  if (IrReceiver.decodedIRData.protocol == NEC) {
    code = IrReceiver.decodedIRData.command;
    buttonStart();
#ifdef debug
    Serial.println("Code: " + String(code));
    Serial.println("Mode: " + String(mode));
    Serial.println("Dir: " + String(dir));
    Serial.println("Scale: " + String(scale));
    Serial.println("Brightness: " + String(brightness));
    Serial.println(String(r) + " " + String(g) + " " + String(b) + " ");
    Serial.println(String(h) + " " + String(s) + " " + String(v) + " ");
    Serial.println();
#endif
  }
}

void buttonStart() {
  if (code < 23 && code >= 0) {
    if (IrReceiver.decodedIRData.flags) {
      if (buttons[code].useRepeat) {
        buttons[code].callback();
      }
    } else {
      buttons[code].callback();
    }
  }
}

void print() {
  Serial.println("");
}

void loop() {
  if (IrReceiver.decode()) {  // If data received
    decodeIR();               // Button handler
    IrReceiver.resume();      // Resume button listening
  }

  switch (mode) {
    case 0:
      staticColorRGB(0, 0, 0);
      break;

    case 1:
      staticColorRGB(r, g, b);
      break;

    case 2:
      staticColorHSV(h, s, v);
      break;

    case 3:
      for (int i = 0; i < LED_NUM; i++) {
        leds[i] = CHSV(counter + i * scale, 255, 255);
      }
      counter++;
      break;

    case 4:
      leds[random(0, LED_NUM)] = CHSV(random(0, 255), 255, 255);
      break;

    case 5:
      if (counter % 5 == 0) {
        staticColorHSV(random(0, 255), 255, 255);
      }
      counter++;
      break;
  }
  
  FastLED.show();

#ifdef debug
  analogWrite(9, leds[0].r);
  analogWrite(5, leds[0].g);
  analogWrite(10, leds[0].b);
#endif
}

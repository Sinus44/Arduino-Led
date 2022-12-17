#include <FastLED.h>
#define FASTLED_ALLOW_INTERRUPTS 1

#include <IRremote.hpp>
#define DECODE_NEC

//#define LED_PIN 2 // REAL USING
//#define IR_PIN 14 // REAL USING

#define LED_PIN 4
#define IR_PIN 3

#define LED_NUM 300

byte mode = 0;
byte modes = 6;
byte counter = 0;
byte brightness = 40;
byte scale = 2;
bool redraw = false;
uint8_t code;

constexpr uint32_t buttons2[4][6] = {
  {
    16187647,
    16195807,
    16191727,
    16199887,
    16189687,
    16197847,
  },
  {
    16220287,
    16228447,
    16224367,
    16232527,
    16222327,
    16230487,
  },
  {
    16203967,
    16212127,
    16208047,
    16216207,
    16206007,
    16214167,
  },
  {
    16236607,
    16244767,
    16240687,
    16248847,
    16238647,
    16246807,
  },
};

constexpr uint8_t buttons[6][4] = {
  {
    0,
    1,
    2,
    3,
  },
  { 4, 5, 6, 7 },
  { 8, 9, 10, 11 },
  { 12, 13, 14, 15 },
  { 16, 17, 18, 19 },
  { 20, 21, 22, 23 },
};

CRGB leds[LED_NUM];


void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_PIN);
  FastLED.addLeds<WS2813, LED_PIN, GRB>(leds, LED_NUM).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
  attachInterrupt(digitalPinToInterrupt(IR_PIN), decodeIR, FALLING);
}

void decodeIR() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == NEC) {
      code = IrReceiver.decodedIRData.command;
      Serial.println("Code: " + String(code));
      checkKey();
      Serial.println("Mode: " + String(mode));
    }
    IrReceiver.resume();
  }
}

void loop() {
  switch (mode) {
    case 5:
      rainbow();
      FastLED.show();
      break;
  }
}

void checkKey() {
  switch (code) {
    case buttons[0][0]: enable(); break;    // OFF or MODE=1
    case buttons[0][1]: setMode(5); break;  // empty
    case buttons[0][2]: prewMode(); break;  // PREW MODE
    case buttons[0][3]: nextMode(); break;  // NEXT MODE

    case buttons[1][0]: setMode(2); break;  // RED
    case buttons[1][1]: setMode(3); break;  // GREEN
    case buttons[1][2]: setMode(4); break;  // BLUE
    case buttons[1][3]: setMode(1); break;  // WHITE

    case buttons[2][0]: scaleMinus(); break;   // SCALE -
    case buttons[2][1]: brightMinus(); break;  // BRIGHTNESS -
    case buttons[2][2]: brightPlus(); break;   // BRIGHTNESS +
    case buttons[2][3]: scalePlus(); break;    // SCALE +
  }
}

void setMode(byte num) {
  Serial.println("Mode set " + String(num));
  mode = num;

  switch (mode) {
    case 0: staticColor(0, 0, 0); break;        // OFF
    case 1: staticColor(255, 255, 255); break;  // WHITE
    case 2: staticColor(255, 0, 0); break;      // RED
    case 3: staticColor(0, 255, 0); break;      // GREEN
    case 4: staticColor(0, 0, 255); break;      // BLUE
    case 5:
      //rainbow();
      redraw = true;
      break;  // RAINBOW
  }
}

void enable() {
  mode = !mode;
}

void scalePlus() {
  if (scale < 10) scale++;
}

void scaleMinus() {
  if (scale > 1) scale--;
}

void brightPlus() {
  if (brightness + 5 < 255) {
    brightness += 5;
  } else {
    brightness = 255;
  }
  FastLED.setBrightness(brightness);
}

void brightMinus() {
  if (brightness - 5 > 0) {
    brightness -= 5;
  } else {
    brightness = 0;
  }
  FastLED.setBrightness(brightness);
}

void nextMode() {
  if (mode < modes - 1) setMode(mode + 1);
}

void prewMode() {
  if (mode > 0) setMode(mode - 1);
}

void staticColor(byte r, byte g, byte b) {
  redraw = false;
  for (int i = 0; i < LED_NUM; i++) {
    leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

void rainbow() {
  for (int i = 0; i < LED_NUM; i++) {
    leds[i] = CHSV(counter + i * scale, 255, 255);
  }
  counter++;
}
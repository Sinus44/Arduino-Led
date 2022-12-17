#include <FastLED.h>
#include <IRremote.h>

#define LED_PIN 4
#define IR_PIN 11

#define LED_NUM 9

byte mode = 0;
byte modes = 6;
byte counter = 0;
byte brightness = 40;
byte scale = 3;
int code;

constexpr int buttons[4][6] = {
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

CRGB leds[LED_NUM];

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_PIN);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
}
void loop() {

  if (IrReceiver.decode()) {
    code = IrReceiver.decodedIRData.decodedRawData;

    switch(code) {
      case buttons[0][0]: mode = 0; break; // OFF

      case buttons[0][1]: mode = 1; break; // RED
      case buttons[1][1]: mode = 2; break; // GREEN
      case buttons[2][1]: mode = 3; break; // BLUE
      case buttons[3][1]: mode = 4; break; // WHITE

      case buttons[0][2]: prewMode(); break; // PREW MODE
      case buttons[3][2]: nextMode(); break; // NEXT MODE

      case buttons[0][3]: scaleMinus(); break; // SCALE -
      case buttons[1][3]: brightMinus(); break; // BRIGHTNESS -
      case buttons[2][3]: brightPlus(); break; // BRIGHTNESS +
      case buttons[3][3]: scalePlus(); break; // SCALE +
    }
		IrReceiver.resume();
   }

  switch(mode) {
    case 0: staticColor(0, 0, 0); break; // off
    case 1: staticColor(0, 255, 255); break; // red
    case 2: staticColor(120, 255, 255); break; // green
    case 3: staticColor(240, 255, 255); break; // blue
    case 4: staticColor(0, 0, 255); break; // white
    case 5: rainbow(); break; // rainbow
  }

  FastLED.show();
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
}

void brightMinus() {
  if (brightness-5 > 0) { 
    brightness -= 5;
  } else {
    brightness = 0;
  }
}

void nextMode() {
  if (mode + 1 < modes) mode++;
}

void prewMode() {
  if (mode + 1 > 0) mode--;
}

void staticColor(byte h, byte s, byte v) {
  for (int i = 0; i < LED_NUM; i++ ) {
    leds[i] = CHSV(h, s, v);
  }
}

void rainbow() {
  for (int i = 0; i < LED_NUM; i++ ) {
    leds[i] = CHSV(counter + i * scale, 255, 255);
  }
  counter++;
}

uint8_t modes = 3; // number of modes
uint8_t code; // Code of button
uint8_t mode = 0; // Current mode
uint8_t brightness = 40; // Current brightness

uint8_t counter = 0; // Iteration counter, for some modes
uint8_t scale = 2; // Scale, for some modes
bool redraw = false; // Using redrawing, for some modes
int8_t dir = 1;

byte r;
byte g;
byte b;


#define LED_PIN 4
#define IR_PIN 3
#define LED_NUM 75

// Connecting library
#include <FastLED.h>
CRGB leds[LED_NUM];
#include <IRremote.hpp>
#include "buttons.h"

// Using NEC remote controller
#define DECODE_NEC

//#define LED_PIN 2 // Use for final firmware
//#define IR_PIN 14 // Use for final firmware
//#define LED_NUM 300 // Use for final firmware


// Variables

void setup() {
	#ifdef DEBUG
		Serial.begin(9600); // Инициализация Serial для отладки
	#endif
	
	IrReceiver.begin(IR_PIN); // Setup pin for IR receiver
	FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM).setCorrection(TypicalLEDStrip); // Create strip
	FastLED.setBrightness(brightness); // Set default brightness
}

void loop() {
	if (IrReceiver.decode()) { // If data received
		decodeIR(); // Button handler
		IrReceiver.resume(); // Resume button listening
	}

	if (IrReceiver.isIdle()) { // Is the data accepted
		switch (mode) {
			case 2:
				rainbow();
				FastLED.show();
				break;
		}
	}
}

void decodeIR() {
	if (IrReceiver.decodedIRData.protocol == NEC) {
		code = IrReceiver.decodedIRData.command;
		buttonStart();
		#ifdef DEBUG
			Serial.println("Code: " + String(code));
			Serial.println("Mode: " + String(mode));
		#endif
	}
}

void buttonStart() {
	if (code <= 23 && code >= 1) {
	if (IrReceiver.decodedIRData.flags) {
		if (buttons[code].useRepeat) {
			buttons[code].callback();
		}
	} else {
		buttons[code].callback();
	}
}
}

void rainbow() {
  for (int i = 0; i < LED_NUM; i++) {
	leds[i] = CHSV(counter + i * scale, 255, 255);
  }
  counter++;
}


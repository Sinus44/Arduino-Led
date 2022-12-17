struct button {
    bool useRepeat;
    void (*callback) ();
};

////////////////////////////////////

void staticColor(uint8_t r, uint8_t g, uint8_t b) {
  redraw = false;
  for (int i = 0; i < LED_NUM; i++) {
	leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

void brightnessChange() {
	if (dir > 0) {
		if (brightness + 5 <= 255) {
			brightness += 5;
		} else {
			brightness = 255;
		}
	} else {
		if (brightness - 5 > 0) {
			brightness -= 5;
		} else {
			brightness = 0;
		}
	}
	FastLED.setBrightness(brightness);
}

void scaleChange() {
	if (dir > 0) {
		if (scale < 10) scale++;
	} else {
		if (scale > 0) scale--;
	}
}

void changeDir() {
	dir *= -1;
}

void enable() {
	mode = !mode;
}

void redChange() {
	mode = 1;
	if (dir > 0) {
		if (r + 5 <= 255) {
			r += 5;
		} else {
			r = 255;
		}
	} else {
		if (r - 5 > 0) {
			r -= 5;
		} else {
			r = 0;
		}
	}
	staticColor(r,g,b);
}

void greenChange() {
	mode = 1;
	if (dir > 0) {
		if (g + 5 <= 255) {
			g += 5;
		} else {
			g = 255;
		}
	} else {
		if (g - 5 > 0) {
			g -= 5;
		} else {
			g = 0;
		}
	}
	staticColor(r,g,b);
}


void blueChange() {
	mode = 1;
	if (dir > 0) {
		if (b + 5 <= 255) {
			b += 5;
		} else {
			b = 255;
		}
	} else {
		if (b - 5 > 0) {
			b -= 5;
		} else {
			b = 0;
		}
	}
	staticColor(r,g,b);
}

void colorMode() {
	if (mode+1 != modes) {
		mode += 1;
	} else {
		mode = 0;
	}

	if (mode==2) {
		redraw = true;
	} else {
		redraw = false;
	}
}

//////////////////////

button buttons[] = {
	{true, brightnessChange},
	{true, scaleChange},
	{false, changeDir},
	{false, enable},

	{true, redChange},
	{true, greenChange},
	{true, blueChange},
	{true, colorMode}

};
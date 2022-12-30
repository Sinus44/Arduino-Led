struct button {
  bool useRepeat;
  void (*callback)();
};

////////////////////////////////////

void staticColorRGB(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < LED_NUM; i++) {
    leds[i] = CRGB(r, g, b);
  }
}

void staticColorHSV(uint8_t h, uint8_t s, uint8_t v) {
    for (int i = 0; i < LED_NUM; i++) {
        leds[i] = CHSV(h, s, v);
    }
}

void brightnessChange() {
    if (dir > 0 & brightness + 5 <= 255) brightness += 5;
    else if (dir < 0 & brightness - 5 >= 0) brightness -= 5;
    FastLED.setBrightness(brightness);
}

void scaleChange() {
  if (dir > 0 & scale <= 255) scale ++;
  else if (dir < 0 & scale >= 0) scale --;
}

void dirPlus() { dir = 1; }
void dirMinus() { dir = -1; }

void redChange() {
    mode = 1;
    if (dir > 0 & r + 5 <= 255) r += 5;
    else if (dir < 0 & r - 5 >= 0) r -= 5;
}

void greenChange() {
    mode = 1;
    if (dir > 0 & g + 5 <= 255) g += 5;
    else if (dir < 0 & g - 5 >= 0) g -= 5;
}

void blueChange() {
    mode = 1;
    if (dir > 0 & b + 5 <= 255) b += 5;
    else if (dir < 0 & b - 5 >= 0) b -= 5;
}

void hueChange() {
    mode = 2;
    h+=5;
}

void saturationChange() {
    mode = 2;
    if (dir > 0 & s + 5 <= 255) s += 5;
    else if (dir < 0 & s - 5 >= 0) s -= 5;
}

void valueChange() {
    mode = 2;
    if (dir > 0 & v + 5 <= 255) v += 5;
    else if (dir < 0 & v - 5 >= 0) v -= 5;
}

void colorMode() {
  if (mode < modes - 1) mode++;
  else mode = 0;
}

//////////////////////

button buttons[] = {
  { true, brightnessChange },
  { true, scaleChange },
  { false, dirMinus },
  { false, dirPlus },

  { true, redChange },
  { true, greenChange },
  { true, blueChange },
  { false, colorMode },

  { true, hueChange },
  { true, saturationChange },
  { true, valueChange },
};

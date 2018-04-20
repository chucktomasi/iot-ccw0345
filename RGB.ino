void initRGB() {

  debug("Initializing RGB LED...");
  OLEDDisplayFooterMsg("Initializing RGB LED...");
  FastLED.addLeds<RGB_LED_MODEL, RGB_LED_PIN, COLOR_ORDER>(led_array, NUM_LEDS);
  FastLED.setBrightness(RGB_LED_BRIGHTNESS);

  blinkColorRGB(0, 0, 255, 5);
  debugln("OK");
  OLEDDisplayBodyMsg("RGB...OK", 1);
  OLEDClearLine("footer", 0);
}

void blinkColorRGB(int red, int green, int blue, int frequency) {

  while (frequency) {

    setColorRGB(red, green, blue);
    delay(200);

    clearColorRGB();
    delay(200);
    frequency--;
  }
}


void setColorRGB(int red, int green, int blue) {

  CRGB color = CRGB(green,red, blue);
  fill_solid(led_array, NUM_LEDS, color);
  FastLED.show();
 
}

void clearColorRGB() {

  CRGB colorBlank = CRGB(0, 0, 0);
  fill_solid(led_array, NUM_LEDS, colorBlank);
  FastLED.show();
}

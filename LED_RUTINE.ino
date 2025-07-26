//Inicializa el arreglo de LEDS
void initLeds(){
  pixels.begin();
}

//Rutina de Inicio de Leds
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal = 0, fadeMax = 100;
  pixels.setBrightness(30);

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for (uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops * 65536;
       firstPixelHue += 256) {

    for (int i = 0; i < pixels.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue, 255,
                                           255 * fadeVal / fadeMax)));
    }

    pixels.show();
    delay(wait);

    if (firstPixelHue < 65536) {                             // First loop,
      if (fadeVal < fadeMax) fadeVal++;                      // fade in
    } else if (firstPixelHue >= ((rainbowLoops - 1) * 65536)) { // Last loop,
      if (fadeVal > 0) fadeVal--;                            // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for (int k = 0; k < whiteLoops; k++) {
    for (int j = 0; j < 256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      pixels.fill(pixels.Color(0, 0, 0, pixels.gamma8(j)));
      pixels.show();
    }
    delay(1000); // Pause 1 second
    for (int j = 255; j >= 0; j--) { // Ramp down 255 to 0
      pixels.fill(pixels.Color(0, 0, 0, pixels.gamma8(j)));
      pixels.show();
    }
  }

  delay(100); // Pause 1/2 second
}

//Asigna al arreglo el estado de la SD, de la conexión WIFI y de la conexión del GPS, además de notificar el contenido de CO2 por colores, según el rango actual de la variable
void indicativeCO2() {

  wifiState();
  sdState();
  GPS_State(); 

  if (flagTimeLeds)
  {
    flagTimeLeds = 0;
    flagCallbackTimer = 1;
    if (data_DB.CO2 <= 800) {
      LEDRutine(100, GOOD_AIR_COLOR);
    } else if (data_DB.CO2 < 1200) {
      LEDRutine(100, ACCEPTABLE_AIR_COLOR);
    } else if (data_DB.CO2 < 1500) {
      LEDRutine(100, BAD_AIR_COLOR);
    } else {
      LEDRutine(100, DANGER_AIR_COLOR);
    }
  }

}

//Realiza la rutina principal de los LEDS, encendiendo y apagando de manera consecutiva los LEDS 2 - 22 (debido a que 0, 1 y 23 notifican los estados del WIFI, la SD y el GPS)
void LEDRutine(uint32_t timer, unsigned int color) {

  intervalLeds = timer;
  unsigned int brillo1 = 30;
  pixels.setBrightness(brillo1);

  setColor(color);
  pixels.fill(pixels.Color(0, 0, 0), 2, 21);
  pixels.setPixelColor(ledNumber, pixels.Color(redColor, greenColor, blueColor));
  pixels.setPixelColor(ledNumber + 1, pixels.Color(redColor, greenColor, blueColor));
  pixels.setPixelColor(ledNumber + 2, pixels.Color(redColor, greenColor, blueColor));
  pixels.show();
  ledNumber++;
  
  if (ledNumber == 22) {
    pixels.setPixelColor(21, pixels.Color(0, 0, 0));  
    pixels.setPixelColor(22, pixels.Color(redColor, greenColor, blueColor));    
    ledNumber = 0;
    pixels.setPixelColor(ledNumber + 2, pixels.Color(redColor, greenColor, blueColor));
  }
}

//Notifica el estado de conexión del dispositivo a internet, a través del LED 23
void wifiState() {
  if (WiFi.isConnected()) {
    pixels.setPixelColor(23, 0, 255, 255);
    pixels.show();
  } else {
    pixels.setPixelColor(23,  255, 0, 255);
    pixels.show();
  }
}

//Notifica el estado de la SD, a través del LED 0
void sdState() {
  if (SdOk) {
    pixels.setPixelColor(0, 0, 255, 255);
    pixels.show();
  } else {
    pixels.setPixelColor(0,  255, 0, 255);
    pixels.show();
  }
}

//Notifica el estado de conexión y lectura del GPS, a través del LED 1
void GPS_State() {
  if ((data_DB.latitud && data_DB.longitud) != 0.0000) {
    pixels.setPixelColor(1, 0, 255, 255);
    pixels.show();
  } else {
    pixels.setPixelColor(1, 255, 0, 255);
    pixels.show();
  }
}

//Notifica el reinicio de la memoria EEPROM
void resetEeprom(){
  if(resetEEprom){
    resetEEprom= false;
    pixels.fill(pixels.Color(255, 255, 255), 0, NUMPIXELS);
    pixels.show();
    delay(1000);
    pixels.fill(pixels.Color(0, 0, 0), 0, NUMPIXELS);
    pixels.show();
  }
}

//
void timerLeds() {
  static uint32_t timerLast = 0;
  uint32_t timerNow = millis();
  if (timerNow - timerLast >= intervalLeds)
  {
    timerLast = timerNow;
    flagTimeLeds = 1;
    flagCallbackTimer = 0;
  }
}

//Configura el color de los LEDS dependiendo de la concentración de CO2 detectada por el sensor
void setColor(unsigned int color) {
  switch (color) {
    case 1: {
        setGoodAirColor();
        break;
      }
    case 2: {
        setAcceptableAirColor();
        break;
      }
    case 3: {
        setBadAirColor();
        break;
      }
    case 4: {
        setDangerAirColor();
        break;
      }
  }
}

//Color de los LEDS para indicar un buen estado de la calidad del aire en concentración de CO2
void setGoodAirColor() {
  redColor = 0;
  greenColor = 255;
  blueColor = 0;
}

//Color de los LEDS para indicar un estado aceptable de la calidad del aire en concentración de CO2
void setAcceptableAirColor() {
  redColor = 255;
  greenColor = 108;
  blueColor = 0;
}

//Color de los LEDS para indicar un mal estado de la calidad del aire en concentración de CO2
void setBadAirColor() {
  redColor = 255;
  greenColor = 40;
  blueColor = 0;
}

//Color de los LEDS para indicar un estado peligroso de la calidad del aire en concentración de CO2
void setDangerAirColor() {
  redColor = 255;
  greenColor = 0;
  blueColor = 0;
}

//Inicializa el arreglo de LEDS
void initIndicator() {
  pixels.fill(pixels.Color(0, 0, 0), 0, NUMPIXELS);
  pixels.show();
}

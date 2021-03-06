/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <SSD1306.h>
#include <FastLED.h>
#include "images.h"
#include "HttpClient.h"
#include "Version.h"
#include "Now.h"

#define LED_LOOP_DELAY 3000 // Blink once every 3s

// config variables (stored in EEPROM)
char ssid[32];
char password[32];
/*
char endpoint[128];
char endpoint_auth[64];
char endpoint_fingerprint[64];
*/

CRGB led_array[NUM_LEDS];

int oled_screen_width = 128;
int oled_screen_height = 64;

SSD1306 display = SSD1306(OLED_ADDRESS, OLED_SDA_PIN, OLED_SDC_PIN);

// configuration from server
int report_interval = 0; // seconds
int sample_interval = 100; // ms
int send_data = 0;

// Other global variables
ESP8266WebServer server(80);
char * netname;
char * netnamePrefix = "iot-";
bool ap_mode = false;

/***
 *
 * Run once at startup
 *
 ***/
void setup() {
	delay(1000);
#ifndef NOSERIAL  
	Serial.begin(115200);
#endif

  debugln("");
  debug("Version: ");
  debugln(VERSION);
  initOLED();
  initBuzzer();
  initRGB();
  initWifi();
  loadConfig();
  buildDeviceName();
  buildURL();
  attemptWifi();
    
  if (WiFi.status() != WL_CONNECTED) {   
    ap_mode = true;
    doAP();             // if we can't connect to configured ssid, launch the AP for configuration
  } else {
    ap_mode = false;
    showWifiSuccess();    
    startServer();      // launch web server for additional configuration
                        // other boot-up stuff goes here (thinger.io...etc.)
                        // registerDevice();
    // sensorInit();       // if we're in normal mode, init sensors

    // advertise mDNS name
    registerMDNS();     // let the network know our name
    displayIPAddress();
    OLEDDisplayFooterMsg("Ready...");
  }
  
}

/***
 *
 * Continuous loop
 *
 ***/
void loop() {

  blinkColorRGB(0, 0, 255, 1);
  delay(LED_LOOP_DELAY);
  server.handleClient();

}

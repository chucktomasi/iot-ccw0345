
void startServer() {
  if (!ap_mode) {
    OLEDDisplayFooterMsg("Starting web server...");
  }
  server.on("/", homeScreen);
  server.on("/config", handleConfig);
  server.on("/reset", handleReset);
  server.on("/restart", handleRestart);
  server.on("/led_green", handleLEDGreen);
  server.on("/led_red", handleLEDRed);
  server.on("/led_blue", handleLEDBlue);
  server.on("/beep", handleBeep);
  server.on("/test_image", handleTestImage);
  server.on("/clear_display", handleClearDisplay);
  server.begin();
  debugln("HTTP server started");
  if (!ap_mode) {
    OLEDDisplayBodyMsg("HTTP...OK", 1);
    OLEDDisplayFooterMsg("Ready...");
  } else {
    OLEDDisplayFooterMsg("Configure this device");
  }
}

/*   
 * Config page @ http://192.168.4.1 in a web browser
 */
void homeScreen() {

  String url = buildURL();

  IPAddress ip;
  ip = WiFi.localIP();

  debug("homeScreen(): netname=");
  debugln(netname);

  String response = "<!doctype html><html><head>";
  response += "<title>IoT Properties - ";
  response += netname;
  response += " - ";
  if (WiFi.status() != WL_CONNECTED) {
    response += "AP mode";
  } else {
    response += "Std mode";
  }
  response += "</title>";
  response += "</head><body>";

// Display the current state table
  response += "<h1>IoT Properties - ";
  response += netname;
  response += "</h1>";

    response += "<p><table>";
    response += "  <tr>";
    response += "    <td>";
    response += "       Version:";
    response += "    </td>";
    response += "    <td>";
    response += "       " + String(VERSION);
    response += "    </td>";
    response += "  </tr>";
    response += "  <tr>";
    response += "    <td>";
    response += "       Web addr:";
    response += "    </td>";
    response += "    <td>";
    response += "       <a href=\"" + url + "\">" + url + "</a>";
    response += "    </td>";
    response += "  <tr>";
    response += "    <td>";
    response += "       IP Address:";
    response += "    </td>";
    response += "    <td>";
    response += "       " + ip.toString();
    response += "    </td>";
    response += "  </tr>";
    response += "</table>";
    response += "<hr />";

    
  if (WiFi.status() != WL_CONNECTED) {
    response += "<h1>AP Mode</h1>";
  }

 
  // Display the WiFi config table

  response += "<h2>Configure</h2>";
  response += "<form method=\"POST\" action=\"/config\">";
  response += "<table>";
  response += "<tr><td>Network SSID</th><td><input type=\"text\" name=\"ssid\" value=\"" + String(ssid) + "\"/></td></tr>";
  response += "<tr><td>Password</th><td><input type=\"password\" name=\"password\" value=\"" + String(password) + "\"/></td></tr>";
  /*
  response += "<tr><td>endpoint</th><td><input type=\"text\" name=\"endpoint\" value=\"" + String(endpoint) + "\" size=\"128\"/></td></tr>";
  response += "<tr><td>endpoint_auth</th><td><input type=\"text\" name=\"endpoint_auth\" value=\"\" size=\"64\"/></td></tr>";
  response += "<tr><td>endpoint_fingerprint</th><td><input type=\"text\" name=\"endpoint_fingerprint\" value=\"" + String(endpoint_fingerprint) + "\" size=\"64\"/></td></tr>";
  */
  response += "</table>";
  response += "<input type=\"submit\" value=\"Configure\"/>";
  response += "</form>";

// Display the test actions
  if (WiFi.status() == WL_CONNECTED) {
    response += "<hr />";
    response += "<h2>Test</h2>";
    response += "  <ul>";
    response += "    <li>";
    response += "       <a href=\"/test_image\">Display Test Image</a>";
    response += "    </li>";
    response += "    <li>";
    response += "       <a href=\"/clear_display\">Clear Display</a>";
    response += "    </li>";
    response += "    <li>";
    response += "      Flash the LED: <a href=\"/led_red\">red</a>";
    response += "      &nbsp;|&nbsp;";
    response += "      <a href=\"/led_green\">green</a>";
    response += "      &nbsp;|&nbsp;";
    response += "      <a href=\"/led_blue\">blue</a>";
    response += "    </li>";
    response += "    <li>";
    response += "      <a href=\"/beep\">Beep the buzzer</a>";
    response += "    </li>";
    response += "    <li>";
    response += "      <a href=\"/restart\">Restart</a>";
    response += "    </li>";
    response += "    <li>";
    response += "      <a href=\"/reset\">Reset</a>";
    response += "    </li>";
    response += "</p>";
  }
  response += "</body></html>";

  server.send(200, "text/html", response);
}

void sendResponse(String arg) {

  String url = buildURL();

  String response = "{\n";
  response +=       "\t\"status\":\"OK\",\n";
  response +=       "\t\"home\":\"" + url + "\",\n";
  response +=       "\t\"endpoint\":\"" + url + arg + "\"\n";
  response +=       "}\n";

  server.send(200,"application/json",response);
}

void handleReset() {

  sendResponse("reset");
  debugln("Resetting...");
  OLEDDisplayFooterMsg("Resetting...");
  delay(1000);
  ESP.reset(); }

void handleRestart() {

  sendResponse("restart");
  debugln("Restarting...");
  OLEDDisplayFooterMsg("Restarting...");
  delay(1000);
  ESP.restart();
}

void handleConfig() {
  
  String url = buildURL();

  String response = "{\"status\":\"OK\"";

  for (int i = 0; i < server.args(); i++) {
    response += ",\"" + server.argName(i) + "\":\"" + server.arg(i) + "\"";
    if (server.argName(i) == "ssid") {
      debug("Found ssid: ");
      debugln(server.arg(i));
      server.arg(i).toCharArray(ssid,server.arg(i).length() + 1);
    }
    if (server.argName(i) == "password") {
      debug("Found password: ");
      debugln(server.arg(i));
      server.arg(i).toCharArray(password,server.arg(i).length() + 1);
    }
  } 

  // store configured values to EEPROM
  saveConfig();

  // Send response back to the requester (browser or REST API)
  response += ",\"endpoint\":\"" + url + "config\"}";
  server.send(200,"application/json", response);
  delay(1000);
  ESP.restart();
}

void handleTestImage() {

  OLEDclear();
  OLEDDrawLogo();
  sendResponse("test_image");
  OLEDDisplayFooterMsg("Ready...");
}

void handleClearDisplay() {

  OLEDclear();
  sendResponse("clear_display");
}

void handleLEDGreen() {

  OLEDDisplayFooterMsg("Blinking LED Green...");
  debugln("handleLEDGreen()...");
  blinkColorRGB(0, 255, 0, 5);
  sendResponse("led_green");
  OLEDDisplayFooterMsg("Ready...");
}

void handleLEDRed() {

  OLEDDisplayFooterMsg("Blinking LED Red...");
  debugln("handleLEDRed()...");
  blinkColorRGB(255, 0, 0, 5);
  sendResponse("led_red");
  OLEDDisplayFooterMsg("Ready...");
}

void handleLEDBlue() {

  OLEDDisplayFooterMsg("Blinking LED Blue...");
  debugln("handleLEDBlue()...");
  blinkColorRGB(0, 0, 255, 5);
  sendResponse("led_blue");
  OLEDDisplayFooterMsg("Ready...");
}

void handleBeep() {

  debugln("handleBeep()...");
  OLEDDisplayFooterMsg("Sounding buzzer...");
  beep(1000);
  sendResponse("beep");
  OLEDDisplayFooterMsg("Ready...");
}

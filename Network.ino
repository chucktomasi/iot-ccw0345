#define NETWORK_ATTEMPTS 20

void initWifi() {
  // turn off AP (if it's still on)
  WiFi.softAPdisconnect(true);
  // switch to pure station
  WiFi.mode(WIFI_STA);

}

// build device/AP name
void buildDeviceName() {

  String s1 = WiFi.macAddress();
  String s2 = netnamePrefix + s1.substring(8);
  s2.replace(":","");
  netname = (char *)malloc(s2.length()+1);
  s2.toCharArray(netname,s2.length()+1);
  // strcpy(netname, s.c_str());
  OLEDDisplayBodyMsg(netname, 0);
  
  debug("Device network name: ");
  debugln(netname);

}

// Return a URL
String buildURL() {
  
  String url = "http://" + String(netname) + ".local/";
  
  return url;
}

void attemptWifi() {

  WiFi.begin(ssid, password);
  int retry = 0;
  debugln("Connecting Wifi");
  OLEDDisplayFooterMsg("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED && retry < NETWORK_ATTEMPTS) {
    blinkColorRGB(255, 255, 0, 1); // blink yellow while trying to connect
    debug(".");
    retry++;
    delay(1000);
  }
}

void showWifiSuccess() {
    // connected to ssid, boot normally
    
    debugln("");
    debugln("WiFi connected");
    debug("IP address: ");
    
    debugln(WiFi.localIP());
    OLEDClearLine("footer", 0);
    OLEDDisplayBodyMsg("WIFI...OK", 1);

    debug("Rapid LED blink...");
    // blink LED rapidly for 3 sec to confirm connection
    blinkColorRGB(0, 0, 255, 5);
    debugln("Done.");    
}

void registerMDNS() {
  
  OLEDDisplayFooterMsg("Registering mDNS...");
  if(MDNS.begin(netname,WiFi.localIP(),120)) {
    debug("mDNS responder started: ");
    debug(netname);
    debugln(".local");

    MDNS.addService("http", "tcp", 80);

    MDNS.update();
    OLEDDisplayBodyMsg("mDNS...OK", 1);
  } else {
    debugln("Error starting mDNS");
    OLEDDisplayBodyMsg("mDNS error", 1); 
  }
  delay(1000);
  OLEDClearLine("footer", 0);

}

void doAP() {

  OLEDDisplayTitleMsg("AP Mode");
  String ssid = "SSID:" + String(netname);
  OLEDDisplayBodyMsg(ssid, 0);
  OLEDDisplayBodyMsg("IP:192.168.4.1", 1);
  setColorRGB(255, 255, 0);

  debugln("");
  debugln("Configuring access point...");

  // turn on AP mode
  WiFi.mode(WIFI_AP);

  WiFi.softAP(netname);

  IPAddress myIP = WiFi.softAPIP();
  delay(1000);
  debug("AP IP address: ");
  debugln(myIP);

  // fire up web server
  startServer();
}

void displayIPAddress() {

  IPAddress ip;
  ip = WiFi.localIP();

  OLEDDisplayBodyMsg(ip.toString(), 1);
}


void initBuzzer() {
  debug("Initializing buzzer...");
  OLEDDisplayFooterMsg("Initializing buzzer...");
  pinMode(BUZZER_PIN, OUTPUT);
  beep(1000);
  debugln("OK");
  OLEDDisplayBodyMsg("BUZZER...OK", 1);
  delay(1000);
  OLEDClearLine("footer", 0);
}

void beep(int dur) {

  digitalWrite(BUZZER_PIN, HIGH);
  delay(dur);
  digitalWrite(BUZZER_PIN, LOW);
}

/***
 *
 * screen layout
 *
 * top line 10pt font for screen title
 * next two rows at 16pt font for user messages
 *    at px 12-28, 32-48
 * Footer row at 52 @10pt font
 * 
 ***/

const int title_line = 0;
const int body_lines[] = {12, 32};
const int footer_line = 52;

/***
*
* initialize the screen
*
***/
void initOLED(void) {

  debug("Initializing OLED...");

  /* getHeight() and getWidth() are not available in the library I use
  oled_screen_height = display.getHeight();
  oled_screen_width  = display.getWidth();
  */

  debug(oled_screen_width);
  debug("x");
  debug(oled_screen_height);

  OLEDclear();
  OLEDDisplayTitleMsg("CCW0345");
  OLEDDisplayBodyMsg("OLED...OK", 1);
  debugln(" OK");
}

/***
 *
 * clear the entire screen
 *
 ***/
void OLEDclear() {

  display.init();
  display.clear();

  display.flipScreenVertically();
  display.setContrast(255);
  display.setColor(WHITE);

  display.setFont(ArialMT_Plain_16);
}

/***
 *
 * display a text message on the title row
 *
 ***/
void OLEDDisplayTitleMsg(String msg) {

  OLEDClearLine("title", 0);
  display.setFont(ArialMT_Plain_10);
  OLEDDisplayText(msg, title_line);
}

/***
 *
 * display a text message on the footer row
 *
 ***/
void OLEDDisplayFooterMsg(String msg) {

  OLEDClearLine("footer", 0);
  display.setFont(ArialMT_Plain_10);
  OLEDDisplayText(msg, footer_line);
}

/***
 *
 * display a text message on one of the body rows
 *
 ***/
void OLEDDisplayBodyMsg(String msg, int line) {

  OLEDClearLine("body", line);
  display.setFont(ArialMT_Plain_16);
  OLEDDisplayText(msg, body_lines[line]);
}

/***
 *
 * clear a named zone
 *
 * line used for body lines only, pass 0 for title and footer
 *
 ***/
void OLEDClearLine(String zone, int line) {

  int y = 0;
  int height = 10;

  if (zone ==  "title") {
      y = title_line;
      height = 12;
  }

  if (zone ==  "body") {
      y = body_lines[line];
      height = 20;
  }

  if (zone ==  "footer") {
      y = footer_line;
      height = 12;
  }

  display.setColor(BLACK);
  display.fillRect(0, y, 128, height);
  display.display();
}

/***
 *
 * display a text message at line X in the font size
 * y is the centerpoint of the text
 *
 ***/
void OLEDDisplayText(String msg, int y) {

  int x = oled_screen_width / 2;

  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(x, y, msg);
  display.display();
}

/***
 *
 * Draw the logo on the screen
 *
 ***/
void OLEDDrawLogo() {
  debugln("Drawing Logo");
  display.drawXbm(0, 0, k18_logo_width, k18_logo_height, k18_logo); 
  display.display();
}


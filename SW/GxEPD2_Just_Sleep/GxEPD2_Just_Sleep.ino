/*
* Example code on eInk/ePaper display
*
* Compile as ESP32 dev module
* ESPink - ESP32 board for ePaper: https://www.laskakit.cz/laskakit-espink-esp32-e-paper-pcb-antenna/
*
* -------- ESPink pinout -------
* MOSI/SDI 23
* CLK/SCK 18
* SS 5 //CS
* DC 17 
* RST 16  
* BUSY 4 
* -------------------------------
*
* Libraries:
* EPD library: https://github.com/ZinggJM/GxEPD2
*
*/

// Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Display ePaper for Arduino: https://forum.arduino.cc/index.php?topic=436411.0

// see GxEPD2_wiring_examples.h for wiring suggestions and examples

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>
// Note: if you use this with ENABLE_GxEPD2_GFX 1:
//       uncomment it in GxEPD2_GFX.h too, or add #include <GFX.h> before any #include <GxEPD2_GFX.h>

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "GxEPD2_display_selection.h"
#include "bitmaps/Bitmaps1304x984.h" // 12.48" b/w

#define SLEEP_SEC 15         // Measurement interval (seconds)
const char HelloWorld[] = "Hello World!";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  delay(100);

// turn on power to display
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);   // turn the Display on (HIGH is the voltage level)
  Serial.println("Display power ON");
  delay(1000);   
  
  display.init(); // inicializace

  // first update should be full refresh
  helloWorld();
  delay(5000);
  Serial.println("setup done");

  goToSleep();
}

void goToSleep(){
  Serial.println("going to sleep " + String(SLEEP_SEC) + " sek");
  // ESP Deep Sleep 
    
  display.powerOff();
  digitalWrite(2, LOW);   // turn the display OFF (LOW is the voltage level)
  Serial.println("Display power OFF");
  Serial.println("ESP in sleep mode");
  Serial.flush(); 
  esp_sleep_enable_timer_wakeup(SLEEP_SEC * 1000000);
  esp_deep_sleep_start();
}

void loop()
{
}


void helloWorld()
{
  //Serial.println("helloWorld");
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  if (display.epd2.WIDTH < 104) display.setFont(0);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloWorld);
  }
  while (display.nextPage());
  Serial.println("helloWorld done");
}

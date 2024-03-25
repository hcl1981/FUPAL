/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

#include "SPI.h"
#include "TFT_eSPI.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <Average.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include "toolkit.h"
#include "mechanik1.h"
#include "mechanik2.h"
#include "waerme.h"
#include "akustik.h"
#include "optik.h"
#include "elektrizitaet.h"
#include "tools.h"

#include "FS.h"
#include "SPIFFS.h"

#define DISPLAY_VERSION 1

#define FORMAT_SPIFFS_IF_FAILED true

//#include "font.h"

#define GFXFF 1

#define SANSBOLD9 &LiberationSans_Bold9pt8b

/*
  TL_DATUM = 0 = Top left
  TC_DATUM = 1 = Top centre
  TR_DATUM = 2 = Top right
  ML_DATUM = 3 = Middle left
  MC_DATUM = 4 = Middle centre
  MR_DATUM = 5 = Middle right
  BL_DATUM = 6 = Bottom left
  BC_DATUM = 7 = Bottom centre
  BR_DATUM = 8 = Bottom right
*/

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

void lizenzen1(TFT_eSPI tft)
{
  const char *m_menu[20];
  m_menu[0] = strdup("ADS1X15 Library by Adafruit");
  m_menu[1] = strdup("BMP280 Library by Adafruit");
  m_menu[2] = strdup("BusIO Library by Adafruit");
  m_menu[3] = strdup("MPU6050 Library by Adafruit");
  m_menu[4] = strdup("Unified Sensor Library by Adafruit");
  m_menu[5] = strdup("Arduino Core for the ESP32 by espressif");
  m_menu[6] = strdup("Average by boangri");
  m_menu[7] = strdup("JPEGDecoder Library by Bodmer");
  m_menu[8] = strdup("TFT_eSPI Library by Bodmer");

 
    switch (drawSelection(m_menu, 9, true, false, tft))
    {
    case 0:
      showtext("/ads1x15.txt", tft);
      break;
    case 1:
      showtext("/bmp280.txt", tft);
      break;
    case 2:
      showtext("/busio.txt", tft);
      break;
    case 3:
      showtext("/mpu6050.txt", tft);
      break;
    case 4:
      showtext("/sensor.txt", tft);
      break;
    case 5:
      showtext("/esp32core.txt", tft);
      break;
    case 6:
      showtext("/average.txt", tft);
      break;
    case 7:
      showtext("/jpegdecoder.txt", tft);
      break;
    case 8:
      showtext("/tftespi.txt", tft);
      break;
    default:
      break;
    }
  
}

void lizenzen2(TFT_eSPI tft)
{
  const char *m_menu[20];
  m_menu[0] = strdup("MD_CirQueue MajicDesigns");
  m_menu[1] = strdup("DallasTemperature by milesburton");
  m_menu[2] = strdup("RF24 by nRF24");

 
    switch (drawSelection(m_menu, 3, true, false, tft))
    {
    case 0:
      showtext("/md_cirqueue.txt", tft);
      break;
    case 1:
      showtext("/dallas.txt", tft);
      break;
    case 2:
      showtext("/rf25.txt", tft);
      break;
    default:
      break;
    }
  
}

void infoscreen(TFT_eSPI tft)
{
  const char *m_menu[20];
  m_menu[0] = strdup("Lizenz (CC by-sa 4.0)");
  m_menu[1] = strdup("Verwendete Software I");
  m_menu[2] = strdup("Verwendete Software II");
  const char *textlines[20];
  textlines[0] = strdup("Fupal GO");
  textlines[1] = strdup("Jan Schulz, 2020");
  textlines[2] = strdup("https://513kid.de");

  
    switch (drawSelectionInfoscreen(textlines, 3, m_menu, 3, true, tft))
    {
    case 0:
      showtext("/lizenz.txt", tft);
      break;
    case 1:
      lizenzen1(tft);
      break;
    case 2:
      lizenzen2(tft);
      break;
    default:
      break;
    }
  
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Start...");

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  pinMode(SW5, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  tft.begin();
  
  if(DISPLAY_VERSION == 1){
    tft.setRotation(3);
  }else{
    tft.setRotation(1);
  }
  drawJpeg("/titel.jpg", 0, 0, tft);
  delay(3000);

  tft.setFreeFont(SANSBOLD9);
  tft.setTextColor(TFT_GREEN);
  tft.fillScreen(TFT_BLACK);

  const char *m_menu[20];

  m_menu[0] = strdup("1. Mechanik I");
  m_menu[1] = strdup("2. Mechanik II");
  m_menu[2] = strdup("3. Wärmelehre");
  m_menu[3] = strdup("4. Akustik");
  m_menu[4] = strdup("5. Optik");
  m_menu[5] = strdup("6. Elektrizitätslehre");
  m_menu[6] = strdup("7. Tools");
  m_menu[7] = strdup("8. Information");

  while (true)
  {
    switch (drawSelection(m_menu, 8, false, false, tft))
    {
    case 0:
      mechanik1(tft);
      break;
    case 1:
      mechanik2(tft);
      break;
    case 2:
      waerme(tft);
      break;
    case 3:
      akustik(tft);
      break;
    case 4:
      optik(tft);
      break;
    case 5:
      elektrizitaet(tft);
      break;
    case 6:
      tools(tft);
      break;
    case 7:
      infoscreen(tft);
      break;
    default:
      break;
    }
  }
}
void loop()
{
}
/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

#include <math.h>
#include <ESP32Lib.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <QMC5883L.h>
#include <VL53L0X.h>
#include <Adafruit_ADS1015.h>
#include <HX711_ADC.h>
#include <MD_AD9833.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ressources/CodePage437_9x16.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Average.h>
#include <MD_CirQueue.h>

#include "lang.h"
#include "ESPscale.h"
#include "toolkit.h"
#include "defines.h"
#include "KBW.h"
#include "KUK.h"
#include "SUA.h"
#include "SUW.h"
#include "DUW.h"
#include "LUS.h"
#include "SUS.h"

static VGA3BitIscale vga;

void v0()
{
  selectionKBW(vga);
}

void v1()
{
  selectionDUW(vga);
}

void v2()
{
  selectionLUS(vga);
}

void v3()
{
  selectionSUS(vga);
}

void v4()
{
  selectionSUW(vga);
}

void v5()
{
  selectionSUA(vga);
}

void v6()
{
  selectionKUK(vga);
}

void setup()
{
  Serial.begin(115200);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  pinMode(SW5, INPUT);

  const char *m_menu[20];

  //m_menu[0] = "1. Kraft und Bewegung";
  m_menu[0] = strdup(MAINMENU_1);
  m_menu[1] = strdup(MAINMENU_2);
  m_menu[2] = strdup(MAINMENU_3);
  m_menu[3] = strdup(MAINMENU_4);
  m_menu[4] = strdup(MAINMENU_5);
  m_menu[5] = strdup(MAINMENU_6);
  m_menu[6] = strdup(MAINMENU_7);
  m_menu[7] = strdup(MAINMENU_8);

  vga.init(vga.MODE640x480, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(CodePage437_9x16);
  vga.setTextColor(vga.RGB(0, 0, 0));
  showImage(strdup("/fupal.bmp"), vga);
  delay(3000);
  vga.clear(WHITE);
  UI ui(vga);

  while (true)
  {
    switch (ui.drawSelection(m_menu, 8, false))
    {
    case 0:
      v0();
      break;
    case 1:
      v1();
      break;
    case 2:
      v2();
      break;
    case 3:
      v3();
      break;
    case 4:
      v4();
      break;
    case 5:
      v5();
      break;
    case 6:
      v6();
      break;
    case 7:
    {
      showImage(strdup("/info.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
    }
    break;
    default:
      break;
    }
  }
}

void loop()
{
}

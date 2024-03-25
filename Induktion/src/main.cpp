/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

#include <Arduino.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 ads;

RF24 radio(9, 10);
const uint64_t pipe = 0xE6E6E6E6E6E6;
const float   multiplier = 3.0F;

char message[10] = "-9999.99";
String sMessage;
int16_t results;

void setup(void) {
  delay(100);
  Serial.begin(115200);
  Wire.begin();

  radio.begin();
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);           // If you want to save power use "RF24_PA_MIN" but keep in mind that reduces the module's range
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2, 15);
  //radio.setCRCLength(RF24_CRC_8); //ist das gut/sinnvoll/funktiomiert es?
  radio.openWritingPipe(pipe);

  ads.setGain(GAIN_ONE);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.begin();
}

void loop(void) {

  float   multiplier = 2.0F;    /* ADS1015 @ +/- 6.144V gain (12-bit results) */
  results = ads.readADC_Differential_0_1();

  sMessage = String(results * multiplier);
  sMessage.toCharArray(message, 10);
  radio.writeFast(message, sizeof(message)); //besser mit timer? oder warten auf millis() +x?
  Serial.println(message);
  delay(2);
}
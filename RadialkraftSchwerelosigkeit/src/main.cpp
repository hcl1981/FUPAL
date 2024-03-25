/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
const float XOFFSET = 0;
const float YOFFSET = 0;
const float ZOFFSET = 0;

char message[32] = "Das ist unwichtig..";
String sMessage;
RF24 radio(9, 10);
const uint64_t pipe = 0xE6E6E6E6E6E6;

void setup(void) {
  delay(100);
  Serial.begin(115200);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);

  radio.begin();
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MIN);           // If you want to save power use "RF24_PA_MIN" but keep in mind that reduces the module's range
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2, 15);
  radio.openWritingPipe(pipe);
}

void loop(void) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  sMessage = String(a.acceleration.x-XOFFSET) + ";" + String(a.acceleration.y-YOFFSET) + ";" + String(a.acceleration.z-ZOFFSET);
  sMessage.toCharArray(message, 32);
  radio.write(message, sizeof(message));
  Serial.println(message);
  delay(20);
}

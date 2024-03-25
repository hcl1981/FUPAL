/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

static float readTemp1;
static float readPress1;
static boolean tempReady1;

//Temperatur Wasserdicht
static void TemperaturTask1( void * pvParameters ) {
  OneWire oneWire(26);
  DallasTemperature tempSensor(&oneWire);
  tempSensor.begin();

  while (true) {
    tempSensor.requestTemperaturesByIndex(0);
    readTemp1 = tempSensor.getTempCByIndex(0);
    tempReady1 = true;
    delay(2000);
  }
}

void temperature1(VGA3BitIscale vga) {
  xTaskCreatePinnedToCore(TemperaturTask1, "Task1", 10000, NULL, 1, &Task1, 0);

  UI ui(vga);
  Table table(vga, ui, 2, 26, true);
  char *cols[10];
  char buf[15];
  cols[0] = strdup(TIM_IN_S);
  cols[1] = strdup(T_IN_C);

  table.setColumns(cols);
  table.draw();
  ui.drawLegend(START_10S, START_1M, START_5M, SAVE, RESET);
  boolean selected = false;
  int interval = 0;
  float lastTemp;

  while (true) {
    selected = false;
    lastTemp = 0;
    while (!selected) {
      if (digitalRead(SW1) == LOW) {
        interval = 10;
        selected = true;
      }
      if (digitalRead(SW2) == LOW) {
        interval = 60;
        selected = true;
      }
      if (digitalRead(SW3) == LOW) {
        interval = 300;
        selected = true;
      }
      if (readTemp1 != lastTemp) {
        sprintf (buf, "%.2f\xF8\x43", readTemp1);
        vga.fillRect(320, (vga.yres / 2) - (vga.font->charHeight * 2), 320, (vga.font->charHeight * 4), WHITE);
        vga.setCursor((vga.xres * 0.75) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 2));
        vga.printScale(buf, 3);
        lastTemp = readTemp1;
      }
    }
    unsigned long starttime = millis();

    for (int i = 0; i < 26; i++) {
      table.setValueDraw(0, i, i * interval);
      table.setValueDraw(1 , i, readTemp1);

      while (millis() < (starttime + (i + 1) * interval * 1000) && selected) {
        if (readTemp1 != lastTemp) {
          sprintf (buf, "%.2f\xF8\x43", readTemp1);
          vga.fillRect(320, (vga.yres / 2) - (vga.font->charHeight * 2), 320, (vga.font->charHeight * 4), WHITE);
          vga.setCursor((vga.xres * 0.75) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 2));
          vga.printScale(buf, 3);
          lastTemp = readTemp1;
        }
        if (digitalRead(SW5) == LOW) {
          i = 100;
          selected = false;
          table.clean();
          table.draw();
        }
        if (digitalRead(SW4) == LOW) {
          char buf[30];
          SDRW sdrw;
          sdrw.mount();
          char fileN [30];
          strcpy(fileN, sdrw.newFilename());
          for (int i = 0; i < table.getRows(); i++) {
            if (table.getData()[0][i] != -1 && table.getData()[1][i] != -1) {
              sprintf (buf, "%.2f, %.2f \n", table.getData()[0][i], table.getData()[1][i]);
              Serial.print("Writing ");
              Serial.print(buf);
              Serial.print(" to ");
              Serial.print(fileN);
              Serial.println(".");
              sdrw.appendFile(fileN, buf);
            }
          }
        }
      }
    }
    while (digitalRead(SW5) == HIGH) {
      if (digitalRead(SW4) == LOW) {
        char buf[30];
        SDRW sdrw;
        sdrw.mount();
        char fileN [30];
        strcpy(fileN, sdrw.newFilename());
        for (int i = 0; i < table.getRows(); i++) {
          if (table.getData()[0][i] != -1 && table.getData()[1][i] != -1) {
            sprintf (buf, "%.2f, %.2f \n", table.getData()[0][i], table.getData()[1][i]);
            Serial.print("Writing ");
            Serial.print(buf);
            Serial.print(" to ");
            Serial.print(fileN);
            Serial.println(".");
            sdrw.appendFile(fileN, buf);
          }
        }
      }
    }
    selected = false;
    table.clean();
    table.draw();
  }
}

//2 Sensoren

static float readTemp2;
static boolean tempReady2;

//Temperatur Wasserdicht
static void TemperaturTask2( void * pvParameters ) {
  OneWire oneWire(5);
  DallasTemperature tempSensor2(&oneWire);
  tempSensor2.begin();

  while (true) {
    tempSensor2.requestTemperaturesByIndex(0);
    readTemp2 = tempSensor2.getTempCByIndex(0);
    tempReady2 = true;
    delay(2000);
  }
}

void temperature2(VGA3BitIscale vga) {
  xTaskCreatePinnedToCore(TemperaturTask1, "Task1", 10000, NULL, 1, &Task1, 0);
  xTaskCreatePinnedToCore(TemperaturTask2, "Task2", 10000, NULL, 1, &Task2, 0);

  UI ui(vga);
  Table table(vga, ui, 3, 26, true);
  char *cols[10];
  char buf[15];
  cols[0] = strdup(TIM_IN_S);
  cols[1] = strdup(T1_IN_C);
  cols[2] = strdup(T2_IN_C);

  table.setColumns(cols);
  table.draw();
  ui.drawLegend(START_10S, START_1M, START_5M, SAVE, RESET);
  boolean selected = false;
  int interval=0;
  float lastTemp1;
  float lastTemp2;

  while (true) {
    selected = false;
    lastTemp1 = 0;
    lastTemp2 = 0;
    while (!selected) {
      if (digitalRead(SW1) == LOW) {
        interval = 10;
        selected = true;
      }
      if (digitalRead(SW2) == LOW) {
        interval = 60;
        selected = true;
      }
      if (digitalRead(SW3) == LOW) {
        interval = 300;
        selected = true;
      }
      if (readTemp1 != lastTemp1) {
        sprintf (buf, "%.2f\xF8\x43", readTemp1);
        vga.fillRect(320, (vga.yres / 2) - (vga.font->charHeight * 3), 320, (vga.font->charHeight * 3), WHITE);
        vga.setCursor((vga.xres * 0.75) - vga.font->charWidth * 1.5 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 3));
        vga.printScale(buf, 3);
        lastTemp1 = readTemp1;
      }
      if (readTemp2 != lastTemp2) {
        sprintf (buf, "%.2f\xF8\x43", readTemp2);
        vga.fillRect(320, (vga.yres / 2), 320, (vga.font->charHeight * 3), WHITE);
        vga.setCursor((vga.xres * 0.75) - vga.font->charWidth * 1.5 * strlen(buf), (vga.yres / 2));
        vga.printScale(buf, 3);
        lastTemp2 = readTemp2;
      }
    }
    unsigned long starttime = millis();

    for (int i = 0; i < 26; i++) {
      table.setValueDraw(0, i, i * interval);
      table.setValueDraw(1 , i, readTemp1);
      table.setValueDraw(2 , i, readTemp2);

      while (millis() < (starttime + (i + 1) * interval * 1000) && selected) {
        if (readTemp1 != lastTemp1) {
          sprintf (buf, "%.2f\xF8\x43", readTemp1);
          vga.fillRect(320, (vga.yres / 2) - (vga.font->charHeight * 3), 320, (vga.font->charHeight * 3), WHITE);
          vga.setCursor((vga.xres * 0.75) - vga.font->charWidth * 1.5 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 3));
          vga.printScale(buf, 3);
          lastTemp1 = readTemp1;
        }
        if (readTemp2 != lastTemp2) {
          sprintf (buf, "%.2f\xF8\x43", readTemp2);
          vga.fillRect(320, (vga.yres / 2), 320, (vga.font->charHeight * 3), WHITE);
          vga.setCursor((vga.xres * 0.75) - vga.font->charWidth * 1.5 * strlen(buf), (vga.yres / 2));
          vga.printScale(buf, 3);
          lastTemp2 = readTemp2;
        }
        if (digitalRead(SW5) == LOW) {
          i = 100;
          selected = false;
          table.clean();
          table.draw();
        }
        if (digitalRead(SW4) == LOW) {
          char buf[50];
          SDRW sdrw;
          sdrw.mount();
          char fileN [30];
          strcpy(fileN, sdrw.newFilename());
          for (int i = 0; i < table.getRows(); i++) {
            if (table.getData()[0][i] != -1 && table.getData()[1][i] != -1 && table.getData()[2][i] != -1) {
              sprintf (buf, "%.2f, %.2f, %.2f \n", table.getData()[0][i], table.getData()[1][i], table.getData()[2][i]);
              Serial.print("Writing ");
              Serial.print(buf);
              Serial.print(" to ");
              Serial.print(fileN);
              Serial.println(".");
              sdrw.appendFile(fileN, buf);
            }
          }
        }
      }
    }
    while (digitalRead(SW5) == HIGH) {
      if (digitalRead(SW4) == LOW) {
        char buf[50];
        SDRW sdrw;
        sdrw.mount();
        char fileN [30];
        strcpy(fileN, sdrw.newFilename());
        for (int i = 0; i < table.getRows(); i++) {
          if (table.getData()[0][i] != -1 && table.getData()[1][i] != -1 && table.getData()[2][i] != -1) {
            sprintf (buf, "%.2f, %.2f, %.2f \n", table.getData()[0][i], table.getData()[1][i], table.getData()[2][i]);
            Serial.print("Writing ");
            Serial.print(buf);
            Serial.print(" to ");
            Serial.print(fileN);
            Serial.println(".");
            sdrw.appendFile(fileN, buf);
          }
        }
      }
    }
    selected = false;
    table.clean();
    table.draw();
  }
}

//Temperatur + Luftdruck
static void BMPTask( void * pvParameters ) {
  Adafruit_BMP280 bmp; // I2C

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  Wire.setClock(10000);
  
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X4,     /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X4,       /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  while (true) {
    float tT = bmp.readTemperature();

    if ((tT > 0) && (tT < 50)) {
      Serial.println(tT);
      readTemp1 = tT;
    }
    float tP = bmp.readPressure();

    if ((tP > 30000) && (tP < 110000)) {
      Serial.println(tP);
      readPress1 = tP;
    }
    delay(50);
  }
}


void temperatureAndPressure(VGA3BitIscale vga) {
  pinMode(SW5, INPUT);
  UI ui(vga);
  ui.drawLegend("", "", "", "", MEASUREMENT);
  char buf[20];
  xTaskCreatePinnedToCore(
    BMPTask,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  //unsigned long int lastTime;

  vga.setCursor((vga.xres / 2) - vga.font->charWidth * (sizeof(START_MEASUREMENT)) * 1.5 , (vga.yres / 2) - (vga.font->charHeight * 4));
  vga.printScale(START_MEASUREMENT, 3);
  delay(200);
  while (true) {
    while (digitalRead(SW5) == HIGH) {}
    //delay(1000);
    vga.fillRect(0, 0, 640, 400, WHITE);
    sprintf (buf, "%.2f\xF8\x43", readTemp1);
    vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 4));
    vga.printScale(buf, 4);
    Serial.print(buf);
    Serial.print("; ");
    sprintf (buf, "%.2f Pa", readPress1);
    vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2));
    vga.printScale(buf, 4);
    Serial.println(buf);
    //lastTime = millis();
  }
}

void selectionDUW(VGA3BitIscale vga) {
  const char *k_menu[5];
  Serial.println("x");
  k_menu[0] = MENU_DUW_1;
  k_menu[1] = MENU_DUW_2;
  k_menu[2] = MENU_DUW_3;
  k_menu[3] = MENU_DUW_4;
  k_menu[4] = MENU_DUW_5;
  vga.setTextColor(vga.RGB(0, 0, 0));
  UI ui(vga);
  switch (ui.drawSelection(k_menu, 3, true)) {
    case 0:
      temperature1(vga);
      break;
    case 1:
      temperature2(vga);
      break;
    case 2:
      temperatureAndPressure(vga);
      break;
    case 3:
      break;
    case 4:
      break;
    case 100:
      showImage(strdup("/temp.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    case 101:
      showImage(strdup("/temp.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    case 102:
      showImage(strdup("/nix.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    default:
      break;
  }
}

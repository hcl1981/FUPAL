/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

HX711_ADC LoadCell(2, 26);

static char readTemp[20];
static boolean tempReady;
static float readKraft;

//Temperatur Wasserdicht
static void TemperaturTask(void *pvParameters)
{
  OneWire oneWire(26);
  DallasTemperature tempSensor(&oneWire);
  tempSensor.begin();

  while (true)
  {
    tempSensor.requestTemperaturesByIndex(0);
    sprintf(readTemp, "%.2f %cC", tempSensor.getTempCByIndex(0), 0xF8);
    tempReady = true;
    delay(2000);
  }
}

void v10(VGA3BitIscale vga)
{
  xTaskCreatePinnedToCore(
      TemperaturTask, /* Task function. */
      "Task1",        /* name of task. */
      10000,          /* Stack size of task */
      NULL,           /* parameter of the task */
      1,              /* priority of the task */
      &Task1,         /* Task handle to keep track of created task */
      0);             /* pin task to core 0 */

  while (true)
  {
    if (tempReady == true)
    {
      vga.clear(vga.RGB(255, 255, 255));
      vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(readTemp), (vga.yres / 2) - (vga.font->charHeight * 2));
      vga.printScale(readTemp, 4);
      tempReady = false;
    }
    else
    {
      delay(1);
    }
  }
}

void illuminance(VGA3BitIscale vga)
{
  Wire.begin();
  LUX lux;
  char buf[20];
  while (true)
  {
    int BH1750address = 0x23;
    uint16_t val = 0;
    lux.BH1750_Init(BH1750address);
    delay(200);

    if (2 == lux.BH1750_Read(BH1750address))
    {
      val = ((lux.buff[0] << 8) | lux.buff[1]) / 1.2;
      Serial.print(val, DEC);
      Serial.println(SLUX);
      vga.clear(vga.RGB(255, 255, 255));
      sprintf(buf, F_LUX, val);
      vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 2));
      vga.printScale(buf, 4);
      delay(1000);
    }
  }
}

//TOF
static int readTOF;
static boolean tofReady;

static void TOFTask(void *pvParameters)
{
  VL53L0X sensor;
  Serial.println("1");
  Wire.begin();
  sensor.init();
  sensor.setTimeout(500);
  sensor.startContinuous();

  while (true)
  {
    readTOF = sensor.readRangeContinuousMillimeters();
    tofReady = true;
  }
}

void distanceTOF(VGA3BitIscale vga)
{
  tofReady = false;
  xTaskCreatePinnedToCore(
      TOFTask, /* Task function. */
      "Task1", /* name of task. */
      10000,   /* Stack size of task */
      NULL,    /* parameter of the task */
      1,       /* priority of the task */
      &Task1,  /* Task handle to keep track of created task */
      0);      /* pin task to core 0 */

  while (true)
  {
    int value;
    int lastX = 0;
    int lastVal = 0;
    for (int i = 0; i < 160; i++)
    {
      while (!tofReady)
      {
        Serial.println(readTOF);
      }
      value = readTOF;
      Serial.println(value);
      vga.line(lastX, vga.yres - (lastVal / 2), i * 4, vga.yres - (value / 2), vga.RGB(0, 0, 0));
      lastX = i * 4;
      lastVal = value;
      tofReady = false;
    }
    vga.clear(vga.RGB(255, 255, 255));
  }
}

//Temperatur + Luftdruck
void v16(VGA3BitIscale vga)
{
  Adafruit_BMP280 bmp;
  char buf[20];
  if (!bmp.begin())
  {
    Serial.println(F(MOD_NOT_CONN));
    while (1)
      ;
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  while (true)
  {
    sprintf(buf, F_TEMP, bmp.readTemperature(), 0xF8);
    vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 4));
    vga.printScale(buf, 4);
    sprintf(buf, P_TEMP, bmp.readPressure());
    vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2));
    vga.printScale(buf, 4);
    delay(1000);
    vga.clear(vga.RGB(255, 255, 255));
  }
}

//Spannung
void voltage(VGA3BitIscale vga)
{
  Adafruit_ADS1115 ads;
  ads.setGain(GAIN_ONE);
  ads.begin();

  int16_t results;
  float multiplier = 0.125F;
  char buf[20];
  while (true)
  {
    results = ads.readADC_Differential_0_1();
    vga.clear(vga.RGB(255, 255, 255));
    sprintf(buf, F_MV, results * multiplier);
    vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 2));
    vga.printScale(buf, 4);
    delay(1000);
  }
}

//Gauss
void magneticFlux(VGA3BitIscale vga)
{
  QMC5883L compass;
  Wire.begin();
  compass.init();
  compass.setSamplingRate(50);
  compass.setRange(8);
  char buf[20];

  while (true)
  {
    int16_t x, y, z, t;
    compass.readRaw(&x, &y, &z, &t);
    Serial.println((1.0 * z) / 3000.0);
    vga.clear(vga.RGB(255, 255, 255));
    sprintf(buf, F_GAUSS, (1.0 * z) / 3000.0);
    vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 2));
    vga.printScale(buf, 4);
    delay(1000);
  }
}

//Kraft
static void KraftTask(void *pvParameters)
{
  while (true)
  {
    delay(100);
    LoadCell.update();
    delay(100);
    readKraft = LoadCell.getData();
  }
}

void force(VGA3BitIscale vga)
{
  char buf[20];
  float calValue;       // calibration value
  calValue = 21421.976; // uncomment this if you want to set this value in the sketch
  //Wert später in EEprom auf Modul Speichern

  vga.printSC(OFFSET, 4);
  LoadCell.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  LoadCell.start(stabilisingtime);
  if (LoadCell.getTareTimeoutFlag())
  {
    Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
  }
  else
  {
    LoadCell.setCalFactor(calValue); // set calibration value (float)
    Serial.println("Startup + tare is complete");
  }

  xTaskCreatePinnedToCore(
      KraftTask,
      "Task1",
      10000,
      NULL,
      1,
      &Task1,
      0);

  while (true)
  {
    vga.clear(vga.RGB(255, 255, 255));
    sprintf(buf, F_N, abs(readKraft));
    vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(buf), (vga.yres / 2) - (vga.font->charHeight * 2));
    vga.printScale(buf, 4);
    delay(1000);
  }
}

void functionGenerator(VGA3BitIscale vga)
{
  pinMode(27, OUTPUT);
  digitalWrite(27, LOW);
  UI ui(vga);
  ui.drawLegend("", "", "", W_FORM, FREQ);
  delay(100);
  while (digitalRead(SW5) == LOW)
  {
  }
  MD_AD9833 AD(5);
  char buf[7];
  char buf2[10];
  char buf3[10];
  strcpy(buf, "00000");
  strcpy(buf2, OFF);
  strcpy(buf3, Z_HZ);
  vga.setCursor((vga.xres * 0.5) - vga.font->charWidth * 2 * strlen(buf2), (vga.yres / 2) - 2 * (vga.font->charHeight * 2));
  vga.printScale(buf2, 4);
  vga.setCursor((vga.xres * 0.5) - vga.font->charWidth * 2 * strlen(buf3), (vga.yres / 2));
  vga.printScale(buf3, 4);
  AD.begin();
  AD.setMode(MD_AD9833::MODE_OFF);
  AD.setFrequency(MD_AD9833::CHAN_0, 0);
  while (true)
  {
    if (digitalRead(SW4) == LOW)
    {
      vga.clear(vga.RGB(255, 255, 255));
      const char *k_menu[4];
      k_menu[0] = OFF;
      k_menu[1] = WF_SINE;
      k_menu[2] = WF_TRI;
      k_menu[3] = WF_SQUARE;
      switch (ui.drawSelection(k_menu, 4, false))
      {
      case 0:
        AD.setMode(MD_AD9833::MODE_OFF);
        digitalWrite(27, HIGH);
        strcpy(buf2, OFF);
        break;
      case 1:
        AD.setMode(MD_AD9833::MODE_SINE);
        digitalWrite(27, HIGH);
        strcpy(buf2, WF_SINE);
        break;
      case 2:
        AD.setMode(MD_AD9833::MODE_TRIANGLE);
        digitalWrite(27, HIGH);
        strcpy(buf2, WF_TRI);
        break;
      case 3:
        AD.setMode(MD_AD9833::MODE_SQUARE1);
        digitalWrite(27, HIGH);
        strcpy(buf2, WF_SQUARE);
        break;
      default:
        break;
      }

      ui.drawLegend("", "", "", W_FORM, FREQ);
      vga.setCursor((vga.xres * 0.5) - vga.font->charWidth * 2 * strlen(buf2), (vga.yres / 2) - 2 * (vga.font->charHeight * 2));
      vga.printScale(buf2, 4);
      vga.setCursor((vga.xres * 0.5) - vga.font->charWidth * 2 * strlen(buf3), (vga.yres / 2));
      vga.printScale(buf3, 4);
      delay(100);
      while (digitalRead(SW4) == LOW)
      {
      }
    }

    if (digitalRead(SW5) == LOW)
    {
      strcpy(buf, ui.numberInput(buf));
      Serial.println(buf);
      AD.setFrequency(MD_AD9833::CHAN_0, (unsigned int)atoi(buf));
      sprintf(buf3, F_HZ, (unsigned int)atoi(buf));
      ui.drawLegend("", "", "", W_FORM, FREQ);
      vga.setCursor((vga.xres * 0.5) - vga.font->charWidth * 2 * strlen(buf2), (vga.yres / 2) - 2 * (vga.font->charHeight * 2));
      vga.printScale(buf2, 4);
      vga.setCursor((vga.xres * 0.5) - vga.font->charWidth * 2 * strlen(buf3), (vga.yres / 2));
      vga.printScale(buf3, 4);
      delay(100);
      while (digitalRead(SW5) == LOW)
      {
      }
    }
  }
}

void lichtschrankeLZDZ(VGA3BitIscale vga)
{
  pinMode(5, INPUT);   // Lichtschranke
  pinMode(26, OUTPUT); // Magnet

  delay(100);
  UI ui(vga);
  Table table(vga, ui, 2, 20, true);
  Graph graph(vga, 2, 20, true);
  //unsigned long t1, t2;
  bool lz = true;
  bool dz = false;
  ui.drawLegend("Laufzeit", "Dunkelzeit", "", "", "Messung");
  while (true)
  {
    digitalWrite(26, HIGH);
    bool p1, p2, p3, p4, p5;
    p1 = (digitalRead(SW1) == LOW);
    p2 = (digitalRead(SW2) == LOW);
    p3 = (digitalRead(SW3) == LOW);
    p4 = (digitalRead(SW4) == LOW);
    p5 = (digitalRead(SW5) == LOW);
    if (p1 || p2 || p3 || p4 || p5)
    {

      if (p1)
      {
        lz = !lz;
        if (lz)
        {
          //vga.fillCircle(64,,10,GREEN);
        }
        else
        {
          //vga.fillCircle(64,,10,BLACK);
        }
      }
      if (p2)
      {
        dz = !dz;
        if (dz)
        {
          //vga.fillCircle(196,,10,GREEN);
        }
        else
        {
          //vga.fillCircle(196,,10,BLACK);
        }
      }

      if (p5)
      {
        if (lz || dz)
        {
          //Ausgabe "Messung"?
          //digitalWrite(26, LOW);
          //t1 = micros();
          //while (digitalRead(5) == before)
          //{
          //}
          //t2 = micros();
          //table.setSelectedValue((t2 - t1) / 1000.0);
        }
        else
        {
          //Anzeige
          //keine Messung gewählt
        }

        //vga.printSCM(MEASUREMENT, 4, -1, true);
      }
      delay(200);
    }
  }
}

void selectionSUA(VGA3BitIscale vga)
{
  const char *k_menu[20];
  Serial.println("x");
  k_menu[0] = MENU_SUA_1;
  k_menu[1] = MENU_SUA_2;
  k_menu[2] = MENU_SUA_3;
  k_menu[3] = MENU_SUA_4;
  k_menu[4] = MENU_SUA_5;
  k_menu[5] = MENU_SUA_6;
  k_menu[6] = MENU_SUA_7;
  k_menu[7] = MENU_SUA_8;
  k_menu[8] = "LS: LZ, DZ";

  vga.setTextColor(vga.RGB(0, 0, 0));
  UI ui(vga);
  switch (ui.drawSelection(k_menu, 9, false))
  {
  case 0:
    illuminance(vga);
    break;
  case 1:
    distanceTOF(vga);
    break;
  case 2:
    v16(vga);
    break;
  case 3:
    v10(vga);
    break;
  case 4:
    voltage(vga);
    break;
  case 5:
    magneticFlux(vga);
    break;
  case 6:
    force(vga);
    break;
  case 7:
    functionGenerator(vga);
    break;
  case 8:
    lichtschrankeLZDZ(vga);
    break;
  default:
    //
    break;
  }
}

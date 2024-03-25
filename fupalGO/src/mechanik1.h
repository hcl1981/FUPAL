/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

#include <MD_CirQueue.h>

void dunkelzeit1(TFT_eSPI tft)
{
  unsigned long t1;
  pinMode(26, INPUT);
  Table table(4, 11, false);
  char *cols[10];
  cols[0] = strdup("Nr.");
  cols[1] = strdup("t in ms");
  cols[2] = strdup("Nr.");
  cols[3] = strdup("t in ms");
  cols[4] = strdup("Nr.");
  cols[5] = strdup("t in ms");
  cols[6] = strdup("Nr.");
  cols[7] = strdup("t in ms");
  table.setColumns(cols);

  table.draw(tft);
  drawLegend("", "", "", "", "Reset", tft);
  while (true)
  {
    for (int k = 0; k < 2; k++)
    {
      for (int i = 0; i < 11; i++)
      {
        while (digitalRead(26) == HIGH)
        {
          if (digitalRead(SW5) == LOW)
          {
            table.clean();
            table.draw(tft);
            i = 0;
            k = 0;
          }
        }
        t1 = micros();
        delay(2);
        while (digitalRead(26) == LOW)
        {
        }
        table.setValueDraw((k * 2), i, (i + 1) + k * 11, tft);
        table.setValueDraw((k * 2) + 1, i, (micros() - t1) / 1000.0, tft);
      }
    }
    while (digitalRead(SW5) == HIGH)
    {
    }
    table.clean();
    table.draw(tft);
  }
}

static volatile unsigned long LS1Stop = 0;
static volatile unsigned long LS1Start = 0;
static volatile boolean LS1Data;
static volatile boolean LS1Meas;
static volatile unsigned long LS2Stop = 0;
static volatile unsigned long LS2Start = 0;
static volatile boolean LS2Data;
static volatile boolean LS2Meas;

MD_CirQueue Q1(5, sizeof(unsigned long int));
MD_CirQueue Q2(5, sizeof(unsigned long int));

static void IRAM_ATTR LS1int()
{
  unsigned long int tim = micros();
  if (digitalRead(26) == HIGH)
  {
    if (tim > LS1Start + 1000 && LS1Stop == 0 && LS1Start > 0)
    {
      LS1Stop = tim;
      unsigned long int temp = LS1Stop - LS1Start;
      Q1.push((uint8_t *)&temp);
      LS1Data = true;
    }
  }
  else
  {
    if (LS1Start == 0)
    {
      LS1Start = tim;
    }
  }
}

static void IRAM_ATTR LS2int()
{
  unsigned long int tim = micros();
  if (digitalRead(5) == HIGH)
  {
    if (tim > LS2Start + 1000 && LS2Stop == 0 && LS2Start > 0)
    {
      LS2Stop = tim;
      unsigned long int temp = LS2Stop - LS2Start;
      Q2.push((uint8_t *)&temp);
      LS2Data = true;
    }
  }
  else
  {
    if (LS2Start == 0)
    {
      LS2Start = tim;
    }
  }
}

void dunkelzeit2(TFT_eSPI tft)
{
  pinMode(26, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(26), LS1int, CHANGE);
  attachInterrupt(digitalPinToInterrupt(5), LS2int, CHANGE);

  Table table(6, 11, false);
  char *cols[6];
  cols[0] = strdup("Nr.");
  cols[1] = strdup("t1 in ms");
  cols[2] = strdup("t2 in ms");
  cols[3] = strdup("Nr.");
  cols[4] = strdup("t1 in ms");
  cols[5] = strdup("ts in ms");

  table.setColumns(cols);
  table.draw(tft);
  drawLegend("", "", "", "", "Reset", tft);

  while (true)
  {

    for (int k = 0; k < 2; k++)
    {
      for (int i = 0; i < 11; i++)
      {
        while (LS1Data == false && LS2Data == false)
        {
          if (digitalRead(SW5) == LOW)
          {
            table.clean();
            table.draw(tft);
            Q1.clear();
            Q2.clear();
            LS1Start = 0;
            LS1Stop = 0;
            LS2Start = 0;
            LS2Stop = 0;
            i = 0;
            k = 0;
          }
        }

        if (!Q1.isEmpty())
        {
          unsigned long pop;
          Q1.pop((uint8_t *)&pop);
          float value = 0.001 * pop;
          table.setValueDraw((k * 3), i, (i + 1) + k * 26, tft);
          table.setValueDraw((k * 3) + 1, i, value, tft);
          Serial.println(String(LS1Stop) + " " + String(LS1Start));
          LS1Start = 0;
          LS1Stop = 0;
          LS1Data = false;
        }
        if (!Q2.isEmpty())
        {
          unsigned long pop;
          Q2.pop((uint8_t *)&pop);
          float value = 0.001 * pop;
          table.setValueDraw((k * 3), i, (i + 1) + k * 26, tft);
          table.setValueDraw((k * 3) + 2, i, value, tft);
          Serial.println(String(LS2Stop) + " " + String(LS2Start));
          LS2Start = 0;
          LS2Stop = 0;
          LS2Data = false;
        }
      }
    }
    while (digitalRead(SW5) == HIGH)
    {
    }
    table.clean();
    table.draw(tft);
    Q1.clear();
    Q2.clear();
    LS1Start = 0;
    LS1Stop = 0;
    LS2Start = 0;
    LS2Stop = 0;
  }
}

void fallzeit(TFT_eSPI tft)
{
  pinMode(5, INPUT); // Lichtschranke
  dacWrite(26, 150);

  bool p1, p2, p3, p4, p5;
  Table table(2, 11, true);
  Graph graph(2, 11, true);
  unsigned long t1, t2;
  char *cols[2];
  cols[0] = strdup("h in cm");
  cols[1] = strdup("t in ms");

  table.setColumns(cols);
  table.setSelection(0, 0);
  table.draw(tft);
  //delay(1000);
  drawLegend("§", "$", "< >", "", "Bearbeiten", tft);
  while (true)
  {
    //digitalWrite(26, HIGH);
    dacWrite(26, 150);
    p1 = (digitalRead(SW1) == LOW);
    p2 = (digitalRead(SW2) == LOW);
    p3 = (digitalRead(SW3) == LOW);
    p4 = (digitalRead(SW4) == LOW);
    p5 = (digitalRead(SW5) == LOW);
    if (p1 || p2 || p3 || p4 || p5)
    {
      if (p1)
        table.down(tft);
      if (p2)
        table.up(tft);
      if (p3)
        table.left(tft);
      if (p5)
      {
        if (table.getSelectedColumn() == 0)
        {
          char buf[8];
          if (table.getSelectedValue() == -1)
          {
            strcpy(buf, "000.00");
          }
          else
          {
            sprintf(buf, "%06.2f", table.getSelectedValue());
            Serial.println(table.getSelectedValue());
            Serial.println(buf);
          }

          float n;
          n = atof(numberInput(buf, tft));
          table.setSelectedValue(n);
        }
        else
        {
          printMessage("Messung", tft);
          boolean before = digitalRead(5);
          //digitalWrite(26, LOW);
          dacWrite(26, 0);
          //ledcWrite(0, 0);
          t1 = micros();
          while (digitalRead(5) == before)
          {
          }
          t2 = micros();
          table.setSelectedValue((t2 - t1) / 1000.0);
        }
        table.draw(tft);
        graph.draw(table.getData(), tft);
      }
      if (table.getSelectedColumn() == 0)
      {
        drawLegend("§", "$", "< >", "", "Bearbeiten", tft);
      }
      else
      {
        drawLegend("§", "$", "< >", "", "Messung", tft);
      }
      delay(200); //Das muss woanders hin!!!
    }
  }
}

void energieerhaltungFadenpendel(TFT_eSPI tft)
{
  pinMode(26, INPUT); // Magnet
  delay(100);
  unsigned long t1;
  Table table(2, 10, true);
  Graph graph(2, 10, true);
  char *cols[10];
  cols[0] = strdup("h in cm");
  cols[1] = strdup("t in ms");

  table.setColumns(cols);
  table.setSelection(0, 0);
  table.draw(tft);
  drawLegend("§", "$", "< >", "", "Bearbeiten", tft);
  while (true)
  {
    bool p1, p2, p3, p4, p5;
    p1 = (digitalRead(SW1) == LOW);
    p2 = (digitalRead(SW2) == LOW);
    p3 = (digitalRead(SW3) == LOW);
    p4 = (digitalRead(SW4) == LOW);
    p5 = (digitalRead(SW5) == LOW);
    if (p1 || p2 || p3 || p4 || p5)
    {
      if (p1)
        table.down(tft);
      if (p2)
        table.up(tft);
      if (p3)
        table.left(tft);
      if (p5)
      {
        if (table.getSelectedColumn() == 0)
        {
          char buf[8];
          if (table.getSelectedValue() == -1)
          {
            strcpy(buf, "000.00");
          }
          else
          {
            sprintf(buf, "%06.2f", table.getSelectedValue());
            Serial.println(table.getSelectedValue());
            Serial.println(buf);
          }

          float n;
          n = atof(numberInput(buf, tft));
          table.setSelectedValue(n);
        }
        else
        {
          printMessage("Messung", tft);
          boolean before = digitalRead(26);
          while (digitalRead(26) == before)
          {
          }
          t1 = micros(); // geht das?
          delay(2);
          while (digitalRead(26) != before)
          {
          }
          table.setSelectedValue((micros() - t1) / 1000.0);
        }
        table.draw(tft);
        graph.draw(table.getData(), tft);
      }

      if (table.getSelectedColumn() == 0)
      {
        drawLegend("§", "$", "< >", "", "Bearbeiten", tft);
      }
      else
      {
        drawLegend("§", "$", "< >", "", "Messung", tft);
      }
      delay(200);
    }
  }
}

void laufzeit(TFT_eSPI tft)
{
  unsigned long t1;
  unsigned long t2;
  float spd;
  char buf[20];
  pinMode(26, INPUT);
  pinMode(5, INPUT);
  pinMode(12, OUTPUT);
  drawLegend("", "", "", "", "Messung", tft);
  delay(200);
  while (true)
  {
    while (digitalRead(SW5) == HIGH)
    {
    }
    tft.fillScreen(TFT_BLACK);
    printMessage("Messung", tft);
    digitalWrite(12, HIGH);
    while (digitalRead(26) == HIGH)
    {
    }
    t1 = micros();
    while (digitalRead(5) == HIGH)
    {
    }
    t2 = micros();

    spd = (t2 - t1) / 1000.0;
    Serial.println(spd);
    tft.fillScreen(TFT_BLACK);
    sprintf(buf, "%.2f ms", spd);
    printMessage(buf, tft);
    drawLegend("", "", "", "", "Messung", tft);
    digitalWrite(12, LOW);
    delay(200);
  }
}

void stroboskopFall(TFT_eSPI tft)
{
  tft.fillScreen(TFT_BLACK);
  pinMode(26, OUTPUT); // Magnet
  pinMode(5, OUTPUT);  // Auslösen
  pinMode(27, OUTPUT); // Focus

  delay(200);

  drawLegend("", "", "", "", "Start", tft);
  while (true)
  {
    digitalWrite(26, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(27, LOW);
    while (digitalRead(SW5) == HIGH)
    {
    }
    digitalWrite(27, HIGH);
    printMessage("3", tft);
    delay(1000);
    tft.fillRect(0, tft.height() / 2 - 43, tft.width(), 50, TFT_BLACK);
    printMessage("2", tft);
    delay(1000);
    tft.fillRect(0, tft.height() / 2 - 43, tft.width(), 50, TFT_BLACK);
    printMessage("1", tft);
    delay(1000);
    digitalWrite(5, HIGH);
    digitalWrite(26, LOW);
    tft.fillRect(0, tft.height() / 2 - 43, tft.width(), 50, TFT_BLACK);
    printMessage("Freier Fall!", tft);
    delay(3000);
    tft.fillRect(0, tft.height() / 2 - 43, tft.width(), 50, TFT_BLACK);
  }
}

void schwerelosigkeit(TFT_eSPI tft)
{
  Average<float> xMean(5);
  Average<float> yMean(5);
  Average<float> zMean(5);

  tft.fillScreen(TFT_BLACK);
  drawLegend("", "", "", "", "", tft);
  char message[32];
  RF24 radio(27, 5);
  const uint64_t pipe = 0xE6E6E6E6E6E6;
  radio.begin();
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);
  radio.setRetries(2, 15);
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  tft.drawLine(20, 147, 460, 147, TFT_GREEN);
  tft.drawLine(20, map(1000, 2500, -2500, 10, 285), 460, map(1000, 2500, -2500, 10, 285), TFT_GREEN);
  tft.drawLine(20, map(-1000, 2500, -2500, 10, 285), 460, map(-1000, 2500, -2500, 10, 285), TFT_GREEN);
  tft.drawLine(20, 20, 20, 285, TFT_GREEN);
  tft.drawString("a in m/s²", 0, 0);
  tft.drawString("t", 450, 149);
  drawString90("0", 0, map(0, 2500, -2500, 10, 285) + tft.textWidth("0") / 2, tft);
  drawString90("10", 0, map(1000, 2500, -2500, 10, 285) + tft.textWidth("10") / 2, tft);
  drawString90("-10", 0, map(-1000, 2500, -2500, 10, 285) + tft.textWidth("-10") / 2, tft);

  int x = 21;
  int y;
  int lastyx = 148;
  int lastyy = 148;
  int lastyz = 148;
  char *ptr;
  while (true)
  {
    //delay(1);
    delayMicroseconds(100);

    while (radio.available())
    {
      radio.read(message, sizeof(message));
      if (strlen(message) == 0)
      {
        tft.fillScreen(TFT_BLACK);
        printMessage("Modul nicht angeschlossen", tft);
        while (true)
        {
        }
      }
      ptr = strtok(message, ";");
      y = map((int)(atof(ptr) * -100), 2500, -2500, 10, 285);
      y = constrain(y, 10, 285);
      xMean.push(y);
      y = xMean.mean();
      tft.drawLine(x, y, x - 1, lastyx, TFT_RED);
      lastyx = y;
      ptr = strtok(NULL, ";");
      y = map((int)(atof(ptr) * 100), 2500, -2500, 10, 285);
      y = constrain(y, 10, 285);
      yMean.push(y);
      y = yMean.mean();
      tft.drawLine(x, y, x - 1, lastyy, TFT_YELLOW);
      lastyy = y;
      ptr = strtok(NULL, ";");
      y = map((int)(atof(ptr) * 100), 2500, -2500, 10, 285);
      y = constrain(y, 10, 285);
      zMean.push(y);
      y = zMean.mean();
      tft.drawLine(x, y, x - 1, lastyz, TFT_BLUE);
      lastyz = y;
      x++;
      if (x > 458)
      {
        x = 21;
        tft.fillRect(0, 0, tft.width(), 285, TFT_BLACK);

        tft.drawLine(20, 147, 460, 147, TFT_GREEN);
        tft.drawLine(20, map(1000, 2500, -2500, 10, 285), 460, map(1000, 2500, -2500, 10, 285), TFT_GREEN);
        tft.drawLine(20, map(-1000, 2500, -2500, 10, 285), 460, map(-1000, 2500, -2500, 10, 285), TFT_GREEN);
        tft.drawLine(20, 20, 20, 285, TFT_GREEN);
        tft.drawString("a in m/s²", 0, 0);
        tft.drawString("t", 450, 149);
        drawString90("0", 0, map(0, 2500, -2500, 10, 285) + tft.textWidth("0") / 2, tft);
        drawString90("10", 0, map(1000, 2500, -2500, 10, 285) + tft.textWidth("10") / 2, tft);
        drawString90("-10", 0, map(-1000, 2500, -2500, 10, 285) + tft.textWidth("-10") / 2, tft);
      }
      delay(5);
    }
  }
}

void mechanik1(TFT_eSPI tft)
{

  const char *m_menu[20];

  m_menu[0] = strdup("1. Dunkelzeit (1LS)"); //OK
  m_menu[1] = strdup("2. Dunkelzeit (2LS)"); //OK
  m_menu[2] = strdup("3. Messwagen");
  m_menu[3] = strdup("4. Laufzeit"); //testen
  m_menu[4] = strdup("5. Stroboskopaufnahme eines Falls");
  m_menu[5] = strdup("6. Dauer des freien Falls"); //OK
  m_menu[6] = strdup("7. Schwerelosigkeit im freien Fall");
  m_menu[7] = strdup("8. Energieerhaltung am Fadenpendel"); //OK

  switch (drawSelection(m_menu, 8, true, true, tft))
  {
  case 0:
    dunkelzeit1(tft);
    break;
  case 1:
    dunkelzeit2(tft);
    break;
  case 2:
    drawJpeg("/nv.jpg", 0, 0, tft);
    waitForButton();
    tft.fillScreen(TFT_BLACK);
    break;
  case 3:
    laufzeit(tft);
    break;
  case 4:
    stroboskopFall(tft);
    break;
  case 5:
    fallzeit(tft);
    break;
  case 6:
    schwerelosigkeit(tft);
    break;
  case 7:
    energieerhaltungFadenpendel(tft);
    break;
  case 100:
        drawJpeg("/dunkelzeit.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  case 101:
        drawJpeg("/dunkelzeit.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  case 102:
        drawJpeg("/nv.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  case 103:
        drawJpeg("/laufzeit.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  case 104:
        drawJpeg("/nv.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  case 105:
        drawJpeg("/freierfall.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  case 106:
        drawJpeg("/slfreierfall.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  case 107:
        drawJpeg("/eefadenpendel.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  
  default:
    break;
  }
}
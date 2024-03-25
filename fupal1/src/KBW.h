/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

static float **tData;
static int tDataLength;

static void save(void *parameter)
{
  char buf[30];
  SDRW sdrw;
  sdrw.mount();
  char fileN[30];
  strcpy(fileN, sdrw.newFilename());
  for (int i = 0; i < tDataLength; i++)
  {
    if (tData[0][i] != -1 && tData[1][i] != -1)
    {
      sprintf(buf, "%.2f, %.2f \n", tData[0][i], tData[1][i]);
      sdrw.appendFile(fileN, buf);
    }
  }
  vTaskDelete(NULL);
}

void fallTime(VGA3BitIscale vga)
{
  pinMode(5, INPUT);   // Lichtschranke
  pinMode(26, OUTPUT); // Magnet

  delay(100);
  UI ui(vga);
  Table table(vga, ui, 2, 20, true);
  Graph graph(vga, 2, 20, true);
  unsigned long t1, t2;
  char *cols[10];
  cols[0] = strdup(DELTA_H_CM); //###
  cols[1] = strdup(T_IN_MS);

  table.setColumns(cols);
  table.setSelection(0, 0);
  table.draw();
  ui.drawLegend("\x1F", "\x1E", "\x11 \x10", SAVE, EDIT);
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
        table.down();
      if (p2)
        table.up();
      if (p3)
        table.left();
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
          n = atof(ui.numberInput(buf));
          table.setSelectedValue(n);
        }
        else
        {
          vga.printSCM(MEASUREMENT, 4, -1, true);
          boolean before = digitalRead(5);
          digitalWrite(26, LOW);
          //ledcWrite(0, 0);
          t1 = micros();
          while (digitalRead(5) == before)
          {
          }
          t2 = micros();
          table.setSelectedValue((t2 - t1) / 1000.0);
        }
        table.draw();
        graph.draw(table.getData());
      }
      if (p4)
      {
        tData = table.getData();
        tDataLength = table.getRows();
        xTaskCreatePinnedToCore(
            save,    /* Function to implement the task */
            "Task1", /* Name of the task */
            10000,   /* Stack size in words */
            NULL,    /* Task input parameter */
            0,       /* Priority of the task */
            &Task1,  /* Task handle. */
            0);      /* Core where the task should run */
        vga.printSCM(SAVED, 4, -1, true);
        delay(1000);
        table.draw();
        graph.draw(table.getData());
      }
      if (table.getSelectedColumn() == 0)
      {
        ui.drawLegend("\x1F", "\x1E", "\x11 \x10", SAVE, EDIT);
      }
      else
      {
        ui.drawLegend("\x1F", "\x1E", "\x11 \x10", SAVE, MEASUREMENT);
      }
      delay(200);
    }
  }
}

void energyConservationPendulum(VGA3BitIscale vga)
{
  pinMode(26, INPUT); // Magnet
  delay(100);
  unsigned long t1;
  UI ui(vga);
  Table table(vga, ui, 2, 10, true);
  Graph graph(vga, 2, 10, true);
  char *cols[10];
  cols[0] = strdup(DELTA_H_CM);
  cols[1] = strdup(T_IN_MS);

  table.setColumns(cols);
  table.setSelection(0, 0);
  table.draw();
  ui.drawLegend("\x1F", "\x1E", "\x11 \x10", SAVE, EDIT);
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
        table.down();
      if (p2)
        table.up();
      if (p3)
        table.left();
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
          n = atof(ui.numberInput(buf));
          table.setSelectedValue(n);
        }
        else
        {
          vga.printSCM(MEASUREMENT, 4, -1, true);
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
        table.draw();
        graph.draw(table.getData());
      }
      if (p4)
      {
        tData = table.getData();
        tDataLength = table.getRows();
        xTaskCreatePinnedToCore(
            save,    /* Function to implement the task */
            "Task1", /* Name of the task */
            10000,   /* Stack size in words */
            NULL,    /* Task input parameter */
            0,       /* Priority of the task */
            &Task1,  /* Task handle. */
            0);      /* Core where the task should run */
        vga.printSCM(SAVED, 4, -1, true);
        delay(1000);
        table.draw();
        graph.draw(table.getData());
      }
      if (table.getSelectedColumn() == 0)
      {
        ui.drawLegend("\x1F", "\x1E", "\x11 \x10", SAVE, EDIT);
      }
      else
      {
        ui.drawLegend("\x1F", "\x1E", "\x11 \x10", SAVE, MEASUREMENT);
      }
      delay(200);
    }
  }
}

//Dunkelzeit 1LS
void darkTime1LightBarrier(VGA3BitIscale vga)
{
  unsigned long t1;
  pinMode(26, INPUT);
  UI ui(vga);
  Table table(vga, ui, 8, 26, false);
  char *cols[10];
  cols[0] = strdup(NUM);
  cols[1] = strdup(T_IN_MS);
  cols[2] = strdup(NUM);
  cols[3] = strdup(T_IN_MS);
  cols[4] = strdup(NUM);
  cols[5] = strdup(T_IN_MS);
  cols[6] = strdup(NUM);
  cols[7] = strdup(T_IN_MS);
  table.setColumns(cols);
  //table.setSelection(0, 0);
  table.draw();
  ui.drawLegend("", "", "", "", RESET);
  while (true)
  {
    for (int k = 0; k < 4; k++)
    {
      for (int i = 0; i < 26; i++)
      {
        while (digitalRead(26) == HIGH)
        {
          if (digitalRead(SW5) == LOW)
          {
            table.clean();
            table.draw();
            i = 0;
            k = 0;
          }
        }
        t1 = micros();
        delay(2);
        while (digitalRead(26) == LOW)
        {
        }
        table.setValueDraw((k * 2), i, (i + 1) + k * 26);
        table.setValueDraw((k * 2) + 1, i, (micros() - t1) / 1000.0);
        //delay(1);
        //table.draw();
      }
    }
    while (digitalRead(SW5) == HIGH)
    {
    }
    table.clean();
    table.draw();
  }
}

//Dunkelzeit 2LS

static volatile unsigned long LS1Stop = 0;
static volatile unsigned long LS1Start = 0;
static volatile boolean LS1Data = false;
static volatile boolean LS1Meas;
static volatile unsigned long LS2Stop = 0;
static volatile unsigned long LS2Start = 0;
static volatile boolean LS2Data = false;
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

void darkTime2LightBarriers(VGA3BitIscale vga)
{
  UI ui(vga);
  pinMode(26, INPUT);
  pinMode(5, INPUT);

  attachInterrupt(digitalPinToInterrupt(26), LS1int, CHANGE);
  attachInterrupt(digitalPinToInterrupt(5), LS2int, CHANGE);

  Table table(vga, ui, 6, 26, false);
  char *cols[6];
  cols[0] = strdup(NUM);
  cols[1] = strdup(T1_IN_MS);
  cols[2] = strdup(T2_IN_MS);
  cols[3] = strdup(NUM);
  cols[4] = strdup(T1_IN_MS);
  cols[5] = strdup(T2_IN_MS);

  table.setColumns(cols);
  table.draw();
  ui.drawLegend("", "", "", "", RESET);

  while (true)
  {
    
    for (int k = 0; k < 2; k++)
    {
      for (int i = 0; i < 26; i++)
      {
        while (LS1Data == false && LS2Data == false)
        {
          if (digitalRead(SW5) == LOW)
          {
            table.clean();
            table.draw();
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
          table.setValueDraw((k * 3), i, (i + 1) + k * 26);
          table.setValueDraw((k * 3) + 1, i, value);
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
          table.setValueDraw((k * 3), i, (i + 1) + k * 26);
          table.setValueDraw((k * 3) + 2, i, value);
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
    table.draw();
    Q1.clear();
    Q2.clear();
    LS1Start = 0;
    LS1Stop = 0;
    LS2Start = 0;
    LS2Stop = 0;
    LS1Data = false;
    LS2Data = false;
  }
}

//Durchschnittsgeschwindikkeit 2LS
void averageSpeed(VGA3BitIscale vga)
{
  unsigned long t1;
  unsigned long t2;
  float spd;
  pinMode(26, INPUT);
  pinMode(5, INPUT);
  //?      digitalWrite(26, HIGH);
  UI ui(vga);
  Table table(vga, ui, 3, 10, true);
  char *cols[10];
  cols[0] = strdup(D_IN_M);
  cols[1] = strdup(T_IN_S);
  cols[2] = strdup(V_IN_MS);

  table.setColumns(cols);
  table.setSelection(0, 0);
  table.draw();
  ui.drawLegend("\x1F", "\x1E", "\x11", "\x10", EDIT);
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
        table.down();
      if (p2)
        table.up();
      if (p3)
        table.left();
      if (p4)
      {
        if (table.getSelectedColumn() == 0)
        {
          table.right();
        }
        else
        {
          table.left();
        }
      }
      if (p5)
      {
        if (table.getSelectedColumn() == 0)
        {
          char buf[8];
          if (table.getSelectedValue() == -1)
          {
            strcpy(buf, "0000.00");
          }
          else
          {
            sprintf(buf, "%07.2f", table.getSelectedValue());
            Serial.println(table.getSelectedValue());
            Serial.println(buf);
          }

          float n;
          n = atof(ui.numberInput(buf));
          table.setSelectedValue(n);
        }
        else
        {
          vga.printSCM(MEASUREMENT, 4, -1, true);
          while (digitalRead(26) == HIGH)
          {
          }
          t1 = micros();
          while (digitalRead(5) == HIGH)
          {
          }
          t2 = micros();

          table.setSelectedValue((t2 - t1) / 1000000.0);
          spd = table.getValue(0, table.getSelectedRow()) * 1000;
          Serial.println(spd);
          Serial.println((t2 - t1) / 1000);
          spd /= ((t2 - t1) / 1000.0);
          Serial.println(spd);
          table.setValue(2, table.getSelectedRow(), spd);
          vga.clear(vga.RGB(255, 255, 255));
        }
        table.draw();
      }

      if (table.getSelectedColumn() == 0)
      {
        ui.drawLegend("\x1F", "\x1E", "\x11", "\x10", EDIT);
      }
      else
      {
        ui.drawLegend("\x1F", "\x1E", "\x11", "\x10", MEASUREMENT);
      }
      delay(200);
    }
  }
}

//Periode Fadenpendel 1LS
void periodThreadPendulum(VGA3BitIscale vga)
{
  unsigned long t1;
  unsigned long middle1;
  unsigned long middle2;
  pinMode(26, INPUT);
  UI ui(vga);
  Table table(vga, ui, 8, 26, false);
  char *cols[10];
  cols[0] = strdup(NUM);
  cols[1] = strdup(T_IN_MS);
  cols[2] = strdup(NUM);
  cols[3] = strdup(T_IN_MS);
  cols[4] = strdup(NUM);
  cols[5] = strdup(T_IN_MS);
  cols[6] = strdup(NUM);
  cols[7] = strdup(T_IN_MS);
  table.setColumns(cols);
  table.draw();
  ui.drawLegend("", "", "", "", RESET);
  for (int k = 0; k < 4; k++)
  {
    for (int i = 0; i < 26; i++)
    {
      while (digitalRead(26) == HIGH)
      {
        if (digitalRead(SW5) == LOW)
        {
          table.clean();
          table.draw();
          i = 0;
          k = 0;
        }
      }
      t1 = micros();
      delay(2);
      while (digitalRead(26) == LOW)
      {
        if (digitalRead(SW5) == LOW)
        {
          table.clean();
          table.draw();
          i = 0;
          k = 0;
        }
      }
      middle1 = (micros() + t1) / 2;
      delay(2);

      while (digitalRead(26) == HIGH)
      {
        if (digitalRead(SW5) == LOW)
        {
          table.clean();
          table.draw();
          i = 0;
          k = 0;
        }
      }
      t1 = micros();
      delay(2);
      while (digitalRead(26) == LOW)
      {
        if (digitalRead(SW5) == LOW)
        {
          table.clean();
          table.draw();
          i = 0;
          k = 0;
        }
      }
      middle2 = (micros() + t1) / 2;
      table.setValueDraw((k * 2), i, (i + 1) + k * 26);
      table.setValueDraw((k * 2) + 1, i, (middle2 - middle1) / 1000.0);
    }
  }
}

//Periode Federpendel 1LS
void periodSpringPendulum(VGA3BitIscale vga)
{
  unsigned long t1;
  unsigned long middle1;
  unsigned long middle2 = 0;
  pinMode(26, INPUT);
  UI ui(vga);
  Table table(vga, ui, 8, 26, false);
  char *cols[10];
  cols[0] = strdup(NUM);
  cols[1] = strdup(T_IN_MS);
  cols[2] = strdup(NUM);
  cols[3] = strdup(T_IN_MS);
  cols[4] = strdup(NUM);
  cols[5] = strdup(T_IN_MS);
  cols[6] = strdup(NUM);
  cols[7] = strdup(T_IN_MS);
  table.setColumns(cols);
  table.draw();
  ui.drawLegend("", "", "", "", RESET);
  for (int k = 0; k < 4; k++)
  {
    for (int i = 0; i < 26; i++)
    {
      while (digitalRead(26) == HIGH)
      {
        if (digitalRead(SW5) == LOW)
        {
          table.clean();
          table.draw();
          i = 0;
          k = 0;
          middle2 = 0;
        }
      }
      t1 = micros();
      delay(2);
      while (digitalRead(26) == LOW)
      {
        if (digitalRead(SW5) == LOW)
        {
          table.clean();
          table.draw();
          i = 0;
          k = 0;
          middle2 = 0;
        }
      }
      middle1 = (micros() + t1) / 2;
      if (middle2 != 0)
      {
        table.setValueDraw((k * 2), i, (i + 1) + k * 26);
        table.setValueDraw((k * 2) + 1, i, (middle1 - middle2) / 1000.0);
      }
      else
      {
        i--;
      }
      middle2 = middle1;
    }
  }
}

void motorSyncsPendulum(VGA3BitIscale vga)
{
  unsigned long t1;
  unsigned long middle;
  unsigned long middle1;
  unsigned long middle2 = 0;
  unsigned long period;
  unsigned long stepHalf;
  int steps = 0;
  char buf[20];
  pinMode(5, OUTPUT);  // Step
  pinMode(27, OUTPUT); // Richtung
  digitalWrite(27, LOW);
  pinMode(26, INPUT);

  Average<unsigned long> timetable(7);
  UI ui(vga);
  while (true)
  {

    vga.clear(vga.RGB(255, 255, 255));
    vga.printSC(M_PERIOD, 2);
    for (int i = 0; i < 7; i++)
    {
      timetable.push(1);
    }

    do
    {
      while (digitalRead(26) == HIGH)
      {
        if (digitalRead(SW5) == LOW)
        {
        }
      }
      t1 = micros();
      delay(2);
      while (digitalRead(26) == LOW)
      {
        if (digitalRead(SW5) == LOW)
        {
        }
      }
      middle1 = (micros() + t1) / 2;
      middle = middle1 - middle2;
      timetable.push(middle);
      middle2 = middle1;
      delay(2);
    } while ((timetable.stddev() / middle) > 0.001);

    period = timetable.mean();
    unsigned long stepTime = period / 200;
    stepHalf = stepTime / 2;

    vga.clear(vga.RGB(255, 255, 255));
    sprintf(buf, "%.2f ms", 0.001 * period);
    vga.printSC(buf, 4);
    ui.drawLegend("", "", "", "", RESET);
    while (micros() < middle1 + period)
    {
    }

    while (digitalRead(SW5) == HIGH)
    {
      digitalWrite(5, HIGH);
      while (micros() % stepTime < stepHalf)
      {
      }
      digitalWrite(5, LOW);
      while (micros() % stepTime >= stepHalf)
      {
      }
      steps = (steps + 1) % 200;
    }

    while (steps < 200)
    {
      digitalWrite(5, HIGH);
      delay(1);
      digitalWrite(5, LOW);
      delay(1);
      steps++;
    }
    steps = 0;
  }
}

void ultrasonicSpringPendulum(VGA3BitIscale vga)
{
  Serial.println(U_SONIC);
  pinMode(26, OUTPUT);
  pinMode(2, INPUT);
  digitalWrite(26, LOW);
  delay(10);
  UI ui(vga);
  ui.drawLegend("-", "-", "-", SAVE, STOP); // Stop wird zu Reset nachdem gedrückt wurde?
  float entfernung;
  int dauer;
  float l_minimum = 0;
  float l_maximum = 20000;
  float minimum = 20000;
  float maximum = 0;
  int lasty = 240;
  int y;
  unsigned long lastTime = 0;

  vga.line(20, 100, 20, 380, vga.RGB(0, 0, 0));
  vga.line(20, 380, 620, 380, vga.RGB(0, 0, 0));
  for (int i = 0; i < 13; i++)
  {
    vga.line(20 + (i * 50), 380, 20 + (i * 50), 385, vga.RGB(0, 0, 0));
  }
  while (true)
  {
    for (int i = 0; i < 600; i++)
    {
      while (lastTime + 5000 > micros())
      {
      }
      lastTime = micros();
      digitalWrite(26, HIGH);
      delay(1);
      digitalWrite(26, LOW);
      dauer = pulseIn(2, HIGH, 3000);
      //delay(1);
      digitalWrite(26, HIGH);
      delay(1);
      digitalWrite(26, LOW);
      dauer += pulseIn(2, HIGH, 3000);
      entfernung = (dauer / 4) / 0.291;
      if (entfernung > maximum && maximum < 20000)
      {
        maximum = entfernung;
      }
      if (entfernung < minimum)
      {
        minimum = entfernung;
      }
      y = 480 - map(entfernung, l_minimum, l_maximum, 100, 380);
      if ((y >= 100 && y <= 380))
      {
        vga.line(20 + i, lasty, 21 + i, y, vga.RGB(0, 0, 0));
        lasty = y;
      }
      Serial.println(millis());
    }
    vga.fillRect(21, 100, 600, 280, vga.RGB(255, 255, 255));
    l_minimum = minimum;
    minimum = 1000000;
    l_maximum = maximum;
    maximum = 0;
  }
}

void zeroGravity(VGA3BitIscale vga)
{
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
  vga.line(20, 240, 620, 240, vga.RGB(0, 0, 0));
  vga.line(20, 20, 20, 460, vga.RGB(0, 0, 0));
  int x = 21;
  int y;
  int lastyx = 240;
  int lastyy = 240;
  int lastyz = 240;
  char *ptr; //vorher in schleife
  while (true)
  {
    while (radio.available())
    {
      radio.read(message, sizeof(message));
      Serial.println(message);
      if (strlen(message) == 0)
      {
        vga.clear(vga.RGB(255, 255, 255));
        vga.printSC(MOD_NOT_CONN, 2);
        while (true)
        {
        }
      }
      ptr = strtok(message, ";");
      y = map((int)(atof(ptr) * -100), 5000, -5000, 20, 460);
      //Serial.print(x);
      //Serial.print(" - ");
      //Serial.println(y);
      vga.line(x, y, x - 1, lastyx, vga.RGB(255, 0, 0));
      lastyx = y;
      ptr = strtok(NULL, ";");
      y = map((int)(atof(ptr) * 100), 5000, -5000, 20, 460);
      //Serial.print(x);
      //Serial.print(" - ");
      //Serial.println(y);
      vga.line(x, y, x - 1, lastyy, vga.RGB(0, 255, 0));
      lastyy = y;
      ptr = strtok(NULL, ";");
      y = map((int)(atof(ptr) * 100), 5000, -5000, 20, 460);
      //Serial.print(x);
      //Serial.print(" - ");
      //Serial.println(y);
      vga.line(x, y, x - 1, lastyz, vga.RGB(0, 0, 255));
      lastyz = y;
      x++;
      if (x > 618)
      {
        x = 21;
        vga.clear(vga.RGB(255, 255, 255));
        vga.line(20, 240, 620, 240, vga.RGB(0, 0, 0));
        vga.line(20, 20, 20, 460, vga.RGB(0, 0, 0));
      }
      delay(10);
    }
  }
}

void radialForceGraph(VGA3BitIscale vga)
{
  Average<float> xMean(5);
  Average<float> yMean(5);
  Average<float> zMean(5);
  ledcSetup(0, 200, 13);
  ledcAttachPin(26, 0);
  ledcWriteTone(0, 0);
  pinMode(2, OUTPUT); // Richtung

  vga.clear(vga.RGB(255, 255, 255));
  UI ui(vga);
  ui.drawLegend("\x1E", "\x1F", Z_R_S, START, STOP);
  char message[32];
  RF24 radio(27, 5);
  const uint64_t pipe = 0xE6E6E6E6E6E6;
  radio.begin();
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);
  radio.setRetries(2, 15);
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  vga.line(20, 240, 620, 240, vga.RGB(0, 0, 0));
  vga.setCursor(4, 4);
  vga.print("a in m/s\xFD");
  vga.setCursor(605, 244);
  vga.print("t");
  vga.setCursor(4, 234);
  vga.print90("0");
  vga.line(20, map(5000, 20000, -20000, 20, 460), 620, map(5000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
  vga.setCursor(4, map(5000, 20000, -20000, 20, 460) - 1);
  vga.print90("50");
  vga.line(20, map(-5000, 20000, -20000, 20, 460), 620, map(-5000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
  vga.setCursor(4, map(-5000, 20000, -20000, 20, 460) + 2);
  vga.print90("-50");
  vga.line(20, map(10000, 20000, -20000, 20, 460), 620, map(10000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
  vga.setCursor(4, map(10000, 20000, -20000, 20, 460) + 3);
  vga.print90("100");
  vga.line(20, map(-10000, 20000, -20000, 20, 460), 620, map(-10000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
  vga.setCursor(4, map(-10000, 20000, -20000, 20, 460) + 12);
  vga.print90("-100");
  vga.line(20, map(15000, 20000, -20000, 20, 460), 620, map(15000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
  vga.setCursor(4, map(15000, 20000, -20000, 20, 460) + 3);
  vga.print90("150");
  vga.line(20, map(-15000, 20000, -20000, 20, 460), 620, map(-15000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
  vga.setCursor(4, map(-15000, 20000, -20000, 20, 460) + 12);
  vga.print90("-150");

  vga.line(20, 20, 20, 455, vga.RGB(0, 0, 0));
  int x = 21;
  int y;
  int lastyx = 240;
  int lastyy = 240;
  int lastyz = 240;
  char *ptr; //vorher in schleife
  int spd = 100;
  char buf[20];
  float usec;
  unsigned long lastchange = 0;
  while (true)
  {
    if (digitalRead(SW1) == LOW && (lastchange + 200) < millis() && spd < 2500)
    {
      if (spd == 0)
      {
        spd += 200;
      }
      else
      {
        spd += 100;
      }

      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, START, STOP);
    }
    if (digitalRead(SW2) == LOW && (lastchange + 200) < millis() && spd >= 300)
    {
      spd -= 100;
      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, START, STOP);
    }

    if (digitalRead(SW4) == LOW)
    {
      spd = 200;
      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, START, STOP);
    }
    if (digitalRead(SW5) == LOW)
    {
      spd = 0;
      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, START, STOP);
    }

    while (radio.available())
    {
      radio.read(message, sizeof(message));
      //Serial.println(message);
      if (strlen(message) == 0)
      {
        vga.clear(vga.RGB(255, 255, 255));
        vga.printSC(MOD_NOT_CONN, 2);
        while (true)
        {
        }
      }
      ptr = strtok(message, ";");
      y = map((int)(atof(ptr) * -100), 20000, -20000, 20, 460);
      xMean.push(y);
      y = xMean.mean();
      //Serial.print(x); Serial.print(" - "); Serial.println(y);
      vga.line(x, y, x - 1, lastyx, vga.RGB(255, 0, 0));
      lastyx = y;
      ptr = strtok(NULL, ";");
      y = map((int)(atof(ptr) * 100), 20000, -20000, 20, 460);
      //Serial.print(x); Serial.print(" - "); Serial.println(y);
      yMean.push(y);
      y = yMean.mean();
      vga.line(x, y, x - 1, lastyy, vga.RGB(0, 255, 0));
      lastyy = y;
      ptr = strtok(NULL, ";");
      y = map((int)(atof(ptr) * 100), 20000, -20000, 20, 460);
      zMean.push(y);
      y = zMean.mean();
      //Serial.print(x); Serial.print(" - "); Serial.println(y);
      vga.line(x, y, x - 1, lastyz, vga.RGB(0, 0, 255));
      lastyz = y;
      x++;
      if (x > 618)
      {
        x = 21;
        //vga.clear(vga.RGB(255, 255, 255));
        vga.fillRect(0, 0, vga.xres, 454, WHITE);

        vga.line(20, 240, 620, 240, vga.RGB(0, 0, 0));
        vga.setCursor(4, 4);
        vga.print("a in m/s\xFD");
        vga.setCursor(605, 244);
        vga.print("t");
        vga.setCursor(4, 234);
        vga.print90("0");
        vga.line(20, map(5000, 20000, -20000, 20, 460), 620, map(5000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
        vga.setCursor(4, map(5000, 20000, -20000, 20, 460) - 1);
        vga.print90("50");
        vga.line(20, map(-5000, 20000, -20000, 20, 460), 620, map(-5000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
        vga.setCursor(4, map(-5000, 20000, -20000, 20, 460) + 2);
        vga.print90("-50");
        vga.line(20, map(10000, 20000, -20000, 20, 460), 620, map(10000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
        vga.setCursor(4, map(10000, 20000, -20000, 20, 460) + 3);
        vga.print90("100");
        vga.line(20, map(-10000, 20000, -20000, 20, 460), 620, map(-10000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
        vga.setCursor(4, map(-10000, 20000, -20000, 20, 460) + 12);
        vga.print90("-100");
        vga.line(20, map(15000, 20000, -20000, 20, 460), 620, map(15000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
        vga.setCursor(4, map(15000, 20000, -20000, 20, 460) + 3);
        vga.print90("150");
        vga.line(20, map(-15000, 20000, -20000, 20, 460), 620, map(-15000, 20000, -20000, 20, 460), vga.RGB(0, 0, 0));
        vga.setCursor(4, map(-15000, 20000, -20000, 20, 460) + 12);
        vga.print90("-150");

        vga.line(20, 20, 20, 455, vga.RGB(0, 0, 0));
      }
      delay(10);
    }
  }
}

void radialForceValue(VGA3BitIscale vga)
{

  Average<float> aMean(50);
  ledcSetup(0, 200, 13);
  ledcAttachPin(26, 0);
  ledcWriteTone(0, 0);
  pinMode(2, OUTPUT); // Richtung
  vga.clear(vga.RGB(255, 255, 255));
  UI ui(vga);
  ui.drawLegend("\x1E", "\x1F", Z_R_S, START, STOP);
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
  float y;
  float printY = 0;
  float lastPrintY = 0;

  char *ptr; //vorher in schleife
  int spd = 100;
  char buf[20];
  static char output[20];
  float usec;
  unsigned long lastOutput = 0;
  unsigned long lastchange = 0;
  while (true)
  {
    if (digitalRead(SW1) == LOW && (lastchange + 200) < millis() && spd < 2500)
    {
      if (spd == 0)
      {
        spd += 200;
      }
      else
      {
        spd += 100;
      }

      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, START, STOP);
    }
    if (digitalRead(SW2) == LOW && (lastchange + 200) < millis() && spd >= 300)
    {
      spd -= 100;
      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, START, STOP);
    }

    if (digitalRead(SW4) == LOW)
    {
      spd = 200;
      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, START, STOP);
    }
    if (digitalRead(SW5) == LOW)
    {
      spd = 0;
      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, START, STOP);
    }

    while (radio.available())
    {
      radio.read(message, sizeof(message));
      if (strlen(message) == 0)
      {
        vga.clear(vga.RGB(255, 255, 255));
        vga.printSC(MOD_NOT_CONN, 2);
        while (true)
        {
        }
      }
      ptr = strtok(message, ";");
      //ptr = strtok(NULL, ";");
      y = (atof(ptr) * -1.0);
      aMean.push(y);
      printY = aMean.mean();
      if ((printY > lastPrintY + 0.01 || printY < lastPrintY - 0.01) && millis() > lastOutput + 300)
      {
        vga.setTextColor(WHITE);
        sprintf(output, A_MS2_FORMAT, lastPrintY);
        vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(output), (vga.yres / 2) - (vga.font->charHeight * 2));
        vga.printScale(output, 4);
        vga.setTextColor(BLACK);
        sprintf(output, A_MS2_FORMAT, printY);
        vga.setCursor((vga.xres / 2) - vga.font->charWidth * 2 * strlen(output), (vga.yres / 2) - (vga.font->charHeight * 2));
        vga.printScale(output, 4);
        lastPrintY = printY;
        lastOutput = millis();
      }
      delay(10);
    }
  }
}

void fallStroboscope(VGA3BitIscale vga)
{
  vga.clear(vga.RGB(255, 255, 255));
  pinMode(26, OUTPUT); // Magnet
  pinMode(5, OUTPUT);  // Auslösen
  pinMode(27, OUTPUT); // Focus

  UI ui(vga);
  vga.clear(BLACK);
  delay(200);
  while (true)
  {
    ui.drawLegend("", "", "", "", TRIGGER);
    digitalWrite(26, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(27, LOW);
    while (digitalRead(SW5) == HIGH)
    {
    }
    digitalWrite(27, HIGH);
    ui.drawLegend("", "", "3", "", "");
    delay(1000);
    ui.drawLegend("", "", "2", "", "");
    delay(1000);
    ui.drawLegend("", "", "1", "", "");
    delay(1000);
    digitalWrite(5, HIGH);
    digitalWrite(26, LOW);
    ui.drawLegend("", "", F_FALL, "", "");
    delay(1000);
  }
}

void timespan2LightBarriers(VGA3BitIscale vga)
{
  unsigned long t1;
  unsigned long t2;
  float spd;
  char buf[20];
  pinMode(26, INPUT);
  pinMode(5, INPUT);
  pinMode(12, OUTPUT);
  UI ui(vga);
  ui.drawLegend("", "", "", "", MEASUREMENT);
  delay(200);
  while (true)
  {
    while (digitalRead(SW5) == HIGH)
    {
    }
    vga.clear(vga.RGB(255, 255, 255));
    vga.printSCM(MEASUREMENT, 4, -1, true);
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
    vga.clear(vga.RGB(255, 255, 255));
    sprintf(buf, "%.2f ms", spd);
    vga.printSCM(buf, 4, -1, false);
    ui.drawLegend("", "", "", "", MEASUREMENT);
    digitalWrite(12, LOW);
    delay(200);
  }
}

void lswagen(VGA3BitIscale vga)
{
  UI ui(vga);
  unsigned long int first;
  char message[32];
  RF24 radio(27, 5);
  const uint64_t pipe = 0xE6E6E6E6E6E6;
  unsigned long int limit;
  char *ptr;
  unsigned long int ret;
  unsigned long int lastTime = 0;
  unsigned long int lastInp = 0;
  Table table(vga, ui, 3, 26, false);
  char *cols[10];
  cols[0] = strdup(T_IN_MS);
  cols[1] = strdup(S_STR);
  cols[2] = strdup(V_STR_S);
  table.setColumns(cols);
  int row;
  int counter;
  radio.begin();
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);
  radio.setRetries(2, 15);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  while (true)
  {
    //leeren
    vga.fillRect(0, 0, vga.xres, 454, WHITE);
    table.clean();
    table.draw();
    ui.drawLegend(T1S, T2S, T5S, T10S, T20S);
    limit = 0;
    first = 0;
    ret = 0;
    row = 0;
    counter = 0;
    lastInp = 0;
    while (limit == 0)
    {
      if (digitalRead(SW1) == LOW)
      {
        limit = 1000000;
        Serial.println(T1S);
      }
      if (digitalRead(SW2) == LOW)
      {
        limit = 2000000;
        Serial.println(T2S);
      }
      if (digitalRead(SW3) == LOW)
      {
        limit = 5000000;
        Serial.println(T5S);
      }
      if (digitalRead(SW4) == LOW)
      {
        limit = 10000000;
        Serial.println(T10S);
      }
      if (digitalRead(SW5) == LOW)
      {
        limit = 20000000;
        Serial.println(T20S);
      }

      while (radio.available())
      {
        radio.read(message, sizeof(message));
        if (strlen(message) == 0)
        {
          Serial.println(MOD_NOT_CONN);
          while (true)
          {
          }
        }
      }
    }
    ui.drawLegend("", "", "", "", STOP);
    while (row < 26)
    {
      if (radio.available())
      {
        radio.read(message, sizeof(message));
        if (strlen(message) == 0)
        {
          Serial.println(MOD_NOT_CONN);
          while (true)
          {
          }
        }
        //Serial.print("message: ");
        //Serial.println(message);
        ret = strtoul(message, &ptr, 10) - first;

        Serial.println(strtoul(message, &ptr, 10) - lastInp);
        lastInp = strtoul(message, &ptr, 10);

        if (ret > 0)
        {

          if (first == 0)
          {
            first = ret;
            ret = 0;
            //Serial.print("First: ");
            //Serial.println(first);
            lastTime = 0;
            /*table.setValueDraw(0, 0, 0);
            table.setValueDraw(1, 0, 0);
            table.setValueDraw(2, 0, 0);
            row++;*/
          }
          else
          {
            counter++;
            //Serial.print("ret: ");
            //Serial.println(ret);
            if (ret > (limit / 26) * (row + 1))
            {
              //Serial.print(ret);
              //Serial.print(" : ");
              //Serial.print(lastTime);
              table.setValueDraw(0, row, ((ret / 2) + (lastTime / 2)) / 1000.0);
              table.setValueDraw(1, row, counter);
              table.setValueDraw(2, row, 1000000.0 / (ret - lastTime));
              row++;
              //Serial.print(((ret / 2) + (lastTime / 2)));
              //Serial.print("; ");
              //Serial.println(4000000.0 / (ret - lastTime));
            }
          }
          lastTime = ret;
        }
      }
    }
    ui.drawLegend(PLOT_S, PLOT_V, VALUES, "", REPEAT);
    while (true)
    {
      if (digitalRead(SW1) == LOW)
      {
        Graph graph(vga, 2, 26, false);
        //graph.draw(table.getData());
        graph.drawColNull(table.getData(), 1, true);
      }
      if (digitalRead(SW2) == LOW)
      {
        Graph graph(vga, 2, 26, false);
        graph.drawColNull(table.getData(), 2, true);
      }

      if (digitalRead(SW3) == LOW)
      {
        vga.fillRect(0, 0, vga.xres, 454, WHITE);
        table.draw();
      }
      if (digitalRead(SW5) == LOW)
      {
        break;
      }
    }
  }
}

int selectionKBW(VGA3BitIscale vga)
{
  const char *k_menu[20];
  k_menu[0] = MENU_KBW_1;
  k_menu[1] = MENU_KBW_2;
  k_menu[2] = MENU_KBW_3;
  k_menu[3] = MENU_KBW_4;
  k_menu[4] = MENU_KBW_5;
  k_menu[5] = MENU_KBW_6;
  k_menu[6] = MENU_KBW_7;
  k_menu[7] = MENU_KBW_8;
  k_menu[8] = MENU_KBW_9;
  k_menu[9] = MENU_KBW_10;
  k_menu[10] = MENU_KBW_11;
  k_menu[11] = MENU_KBW_12;
  k_menu[12] = MENU_KBW_13;
  k_menu[13] = MENU_KBW_14;
  k_menu[14] = MENU_KBW_15;

  k_menu[14] = MENU_KBW_15;
  vga.setTextColor(vga.RGB(0, 0, 0));
  UI ui(vga);
  while (true)
  {
    switch (ui.drawSelection(k_menu, 15, true))
    {
    case 0:
      darkTime1LightBarrier(vga);
      break;
    case 1:
      darkTime2LightBarriers(vga);
      break;
    case 2:
      lswagen(vga);
      break;
    case 3:
      timespan2LightBarriers(vga);
      break;
    case 4:
      averageSpeed(vga);
      break;
    case 5:
      fallStroboscope(vga);
      break;
    case 6:
      fallTime(vga);
      break;
    case 7:
      zeroGravity(vga);
      break;
    case 8:
      energyConservationPendulum(vga);
      break;
    case 9:
      periodThreadPendulum(vga);
      break;
    case 10:
      periodSpringPendulum(vga);
      break;
    case 11:
      radialForceGraph(vga);
      break;
    case 12:
      radialForceValue(vga);
      break;
    case 13:
      ultrasonicSpringPendulum(vga);
      break;
    case 14:
      motorSyncsPendulum(vga);
      break;

    case 100:
    {
      showImage(strdup("/dunkelzeit.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }
    case 101:
    {
      showImage(strdup("/dunkelzeit.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }
    case 102:
    {
      showImage(strdup("/nix.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }
    case 103:
    {
      showImage(strdup("/laufzeit.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }
    case 104:
    {
      showImage(strdup("/durchschnittsg.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }
    case 105:
    {
      showImage(strdup("/nix.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }
    case 106:
    {
      showImage(strdup("/ffall.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }
    case 107:
    {
      showImage(strdup("/slfall.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }

    case 108:
    {
      showImage(strdup("/eependel.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }

    case 109:
    {
      showImage(strdup("/periode.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }

    case 110:
    {
      showImage(strdup("/periode.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }

    case 111:
    {
      showImage(strdup("/radial.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }

    case 112:
    {
      showImage(strdup("/radial.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }

    case 113:
    {
      showImage(strdup("/ultras.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }

    case 114:
    {
      showImage(strdup("/sync.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
    }
    default:
      //
      break;
    }
  }
}

/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

void stopwatch(VGA3BitIscale vga)
{
  vga.setCursor((vga.xres / 2) - vga.font->charWidth * 18, 20);
  vga.printScale("000:00.00", 4);
  char buf[20];
  char bufBefore[20];
  int xpos;
  int i;
  strcpy(bufBefore, "-----------");

  int line = 0;
  long lastZW = 0;
  long time1 = 0;
  long offset = 0;
  boolean ru = false;
  long msec;
  long secs;
  long mins;
  long restsecs;
  long resthundrets;

  UI ui(vga);
  ui.drawLegend(START, PAUSE, RESET, SPLIT, DELETE);

  while (true)
  {
    if (ru)
    {
      msec = millis() - time1;
      secs = msec / 1000;
      mins = secs / 60;
      restsecs = secs % 60;
      resthundrets = (msec % 1000) / 10;
      sprintf(buf, "%03ld:%02ld.%02ld", mins, restsecs, resthundrets);
      Serial.println(buf);

      xpos = (vga.xres / 2) - vga.font->charWidth * 18;
      vga.setCursor(xpos, 20);
      for (i = 0; i < 9; i++)
      {
        if (buf[i] != bufBefore[i])
        {
          vga.setTextColor(WHITE);
          vga.drawCharScale(xpos + (i * 4 * vga.font->charWidth), 20, 4, bufBefore[i]);
          vga.setTextColor(BLACK);
          vga.drawCharScale(xpos + (i * 4 * vga.font->charWidth), 20, 4, buf[i]);
          bufBefore[i] = buf[i];
        }
      }

      if (digitalRead(SW2) == LOW)
      {
        ru = false;
        offset = (millis() - time1);
      }
      if (digitalRead(SW4) == LOW && line < 20)
      {
        if (millis() > lastZW + 100)
        {
          vga.setCursor((vga.xres / 2) - vga.font->charWidth * 4.5, 80 + vga.font->charHeight * line);
          vga.print(buf);
          line++;
          lastZW = millis();
        }
      }
    }
    else
    {
      if (digitalRead(SW1) == LOW)
      {
        ru = true;
        time1 = millis() - offset;
      }
      if (digitalRead(SW3) == LOW && line < 20)
      {
        offset = 0;
        vga.fillRect(0, 0, vga.xres, 10 + vga.font->charHeight * 4, WHITE);
        vga.setCursor((vga.xres / 2) - vga.font->charWidth * 18, 20);
        vga.printScale("000:00.00", 4);
        strcpy(buf, "000:00.00");
        strcpy(bufBefore, "000:00.00");
      }
      if (digitalRead(SW4) == LOW)
      {
        if (millis() > lastZW + 100)
        {
          vga.setCursor((vga.xres / 2) - vga.font->charWidth * 4.5, 80 + vga.font->charHeight * line);
          vga.print(buf);
          line++;
          lastZW = millis();
        }
      }
    }

    if (digitalRead(SW5) == LOW)
    {
      vga.fillRect(0, 80, vga.xres, vga.font->charHeight * (line + 2), WHITE);
      line = 0;
    }
  }
}

void timer(VGA3BitIscale vga)
{
  vga.setCursor((vga.xres / 2) - vga.font->charWidth * 12, (vga.yres / 2) - vga.font->charHeight * 4 / 2);
  vga.printScale("000:00", 4);
  char buf[20];
  char bufBefore[20];
  strcpy(bufBefore, "-----------");
  int i;
  boolean soundOn = false;
  ledcSetup(0, 1E5, 12);
  ledcAttachPin(15, 0);
  pinMode(12, OUTPUT);
  int xpos = (vga.xres / 2) - vga.font->charWidth * 12;
  long timeToGo = 0;
  long time1 = 0;
  boolean ru = false;
  boolean alarmOn = false;
  unsigned long millisEnd;
  unsigned long int startPause = 0;
  unsigned long int gesPause = 0;
  long msec;
  long secs;
  long mins;
  long restsecs;
  char timeString[7];
  char timeStringA[7];
  strcpy(timeString, "000:00");

  UI ui(vga);
  ui.drawLegend(START, PAUSE, SET, "", "");

  while (true)
  {
    if (ru)
    {
      msec = (timeToGo) - (millis() - time1);
      if (msec <= 0)
      {
        ru = false;
        alarmOn = true;
      }
      secs = msec / 1000;
      mins = secs / 60;
      restsecs = secs % 60;

      sprintf(buf, "%03ld:%02ld", mins, restsecs);

      for (i = 0; i < 6; i++)
      {
        if (buf[i] != bufBefore[i])
        {
          vga.setTextColor(WHITE);
          vga.drawCharScale(xpos + (i * 4 * vga.font->charWidth), (vga.yres / 2) - vga.font->charHeight * 4 / 2, 4, bufBefore[i]);
          vga.setTextColor(BLACK);
          vga.drawCharScale(xpos + (i * 4 * vga.font->charWidth), (vga.yres / 2) - vga.font->charHeight * 4 / 2, 4, buf[i]);
          bufBefore[i] = buf[i];
        }
      }

      if (digitalRead(SW2) == LOW)
      {
        ru = false;
        startPause = millis();
      }
      if (digitalRead(SW4) == LOW)
      {
      }
    }
    else
    {
      if (alarmOn)
      {
        millisEnd = millis();
        while (digitalRead(SW1) == HIGH && digitalRead(SW2) == HIGH && digitalRead(SW3) == HIGH && digitalRead(SW4) == HIGH && digitalRead(SW5) == HIGH)
        {
          if ((millis() - millisEnd) % 600 < 15)
          {
            if (!soundOn)
            {
              ledcWriteTone(0, 2300);
              digitalWrite(12, HIGH);
              soundOn = true;
            }
          }
          else if (((millis() - millisEnd) % 600 > 110) && ((millis() - millisEnd) % 600 < 125))
          {
            if (!soundOn)
            {
              ledcWriteTone(0, 2300);
              digitalWrite(12, HIGH);
              soundOn = true;
            }
          }
          else
          {
            ledcWriteTone(0, 0);
            digitalWrite(12, LOW);
            soundOn = false;
          }
        }

        ledcWriteTone(0, 0);
        digitalWrite(12, LOW);
        alarmOn = false;
        vga.setTextColor(WHITE);
        vga.printSC("000:00", 4);
        vga.setTextColor(BLACK);
        vga.printSC(timeString, 4);
        delay(200);
      }
      if (digitalRead(SW1) == LOW)
      {
        ru = true;
        if (startPause > 0)
        {
          Serial.println(gesPause / 1000);
          time1 += (millis() - startPause);
          startPause = 0;
        }
        else
        {
          time1 = millis();
        }
      }

      if (digitalRead(SW3) == LOW)
      {
        Serial.println(timeString);
        char *timeStringInp = ui.numberInput(timeString);
        int i = 0;
        Serial.println(timeString);
        while (*timeStringInp)
        {
          timeString[i] = *timeStringInp;
          timeStringA[i] = *timeStringInp;
          timeStringInp++;
          i++;
        }
        vga.setCursor((vga.xres / 2) - vga.font->charWidth * 12, (vga.yres / 2) - vga.font->charHeight * 4 / 2);
        vga.printScale(timeString, 4);
        timeToGo = 60 * 1000 * atoi(strtok(timeStringA, ":"));
        timeToGo += 1000 * atoi(strtok(NULL, ":"));
        ui.drawLegend(START, PAUSE, SET, "", "");
      }
    }
  }
}

void selectionKUK(VGA3BitIscale vga)
{
  const char *k_menu[2];
  k_menu[0] = MENU_KUK_1;
  k_menu[1] = MENU_KUK_2;
  vga.setTextColor(vga.RGB(0, 0, 0));
  UI ui(vga);
  switch (ui.drawSelection(k_menu, 2, false))
  {
  case 0:
    stopwatch(vga);
    break;
  case 1:
    timer(vga);
    break;
  default:
    break;
  }
}
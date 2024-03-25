/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

void stoppuhr(TFT_eSPI tft)
{
  tft.setTextDatum(ML_DATUM);
  char buf[20] = "000:00.00";
  char bufBefore[20];
  int pos = (tft.width() / 2 - tft.textWidth(buf) / 2);
  for (int i = 0; i < 9; i++)
  {
    tft.setTextColor(TFT_GREEN);
    tft.drawString(String(buf[i]), pos, 9, GFXFF);
    bufBefore[i] = buf[i];
    pos += (tft.textWidth(String(buf[i])) + 1);
  }

  tft.drawLine(0, 25, 480, 25, TFT_GREEN);
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

  drawLegend("Start", "Pause", "Reset", "Split", "Löschen", tft);

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

      pos = (tft.width() / 2 - tft.textWidth(buf) / 2);

      for (int i = 0; i < 9; i++)
      {
        if (buf[i] == bufBefore[i])
        {
          tft.setTextColor(TFT_GREEN);
          tft.drawString(String(buf[i]), pos, 9, GFXFF);
        }
        else
        {
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.drawString(String(buf[i]), pos, 9, GFXFF);
        }
        bufBefore[i] = buf[i];
        pos += (tft.textWidth(String(buf[i])) + 1);
      }

      if (digitalRead(SW2) == LOW)
      {
        ru = false;
        offset = (millis() - time1);
      }
      if (digitalRead(SW4) == LOW && line < 12)
      {
        if (millis() > lastZW + 100)
        {
          pos = (tft.width() / 2 - tft.textWidth(buf) / 2);
          for (int i = 0; i < 9; i++)
          {
            tft.setTextColor(TFT_GREEN);
            tft.drawString(String(buf[i]), pos, 35 + line * 22, GFXFF);
            bufBefore[i] = buf[i];
            pos += (tft.textWidth(String(buf[i])) + 1);
          }
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
        tft.fillRect(150, 0, tft.width() - 300, 24, TFT_BLACK);
        strcpy(buf, "000:00.00");
        strcpy(bufBefore, "000:00.00");
        pos = (tft.width() / 2 - tft.textWidth(buf) / 2);
        for (int i = 0; i < 9; i++)
        {
          tft.setTextColor(TFT_GREEN);
          tft.drawString(String(buf[i]), pos, 9, GFXFF);
          bufBefore[i] = buf[i];
          pos += (tft.textWidth(String(buf[i])) + 1);
        }
      }
      if (digitalRead(SW4) == LOW)
      {
        if (millis() > lastZW + 100)
        {
          pos = (tft.width() / 2 - tft.textWidth(buf) / 2);
          for (int i = 0; i < 9; i++)
          {
            tft.setTextColor(TFT_GREEN);
            tft.drawString(String(buf[i]), pos, 35 + line * 22, GFXFF);
            bufBefore[i] = buf[i];
            pos += (tft.textWidth(String(buf[i])) + 1);
          }
          line++;
          lastZW = millis();
        }
      }
    }

    if (digitalRead(SW5) == LOW)
    {
      tft.fillRect(150, 26, tft.width() - 300, 265, TFT_BLACK);
      line = 0;
    }
  }
}

void timer(TFT_eSPI tft)
{
  tft.setTextDatum(ML_DATUM);
  char buf[7] = "000:00";
  char bufBefore[20];
  int pos = (tft.width() / 2 - tft.textWidth(buf) / 2);
  for (int i = 0; i < 6; i++)
  {
    tft.setTextColor(TFT_GREEN);
    tft.drawString(String(buf[i]), pos, tft.height() / 2 - 23, GFXFF);
    bufBefore[i] = buf[i];
    pos += (tft.textWidth(String(buf[i])) + 1);
  }

  strcpy(bufBefore, "-----------");
  boolean soundOn = false;
  ledcSetup(0, 1E5, 12);
  ledcAttachPin(15, 0);
  pinMode(12, OUTPUT);
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

  drawLegend("Start", "Pause", "Setzen", "", "", tft);

  while (true)
  {
    if (ru)
    {
      msec = (timeToGo) - (millis() - time1);
      //msec -= gesPause;
      if (msec <= 0)
      {
        ru = false;
        alarmOn = true;
      }
      secs = msec / 1000;
      mins = secs / 60;
      restsecs = secs % 60;

      sprintf(buf, "%03ld:%02ld", mins, restsecs);

      pos = (tft.width() / 2 - tft.textWidth(buf) / 2);

      for (int i = 0; i < 6; i++)
      {
        if (buf[i] == bufBefore[i])
        {
          tft.setTextColor(TFT_GREEN);
          tft.drawString(String(buf[i]), pos, tft.height() / 2 - 23, GFXFF);
        }
        else
        {
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.drawString(String(buf[i]), pos, tft.height() / 2 - 23, GFXFF);
        }
        bufBefore[i] = buf[i];
        pos += (tft.textWidth(String(buf[i])) + 1);
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
        int pos = (tft.width() / 2 - tft.textWidth(timeString) / 2);
        for (int i = 0; i < 6; i++)
        {
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.drawString(String(timeString[i]), pos, tft.height() / 2 - 23, GFXFF);
          bufBefore[i] = buf[i];
          pos += (tft.textWidth(String(timeString[i])) + 1);
        }
        delay(200);
      }

      if (digitalRead(SW1) == LOW)
      {
        ru = true;
        if (startPause > 0)
        {
          //gesPause +=
          //startPause = 0;
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
        char *timeStringInp = numberInput(timeString, tft);
        int i = 0;
        Serial.println(timeString);

        while (*timeStringInp)
        {
          timeString[i] = *timeStringInp;
          timeStringA[i] = *timeStringInp;
          timeStringInp++;
          i++;
        }

        int pos = (tft.width() / 2 - tft.textWidth(timeString) / 2);
        for (int i = 0; i < 6; i++)
        {
          tft.setTextColor(TFT_GREEN);
          tft.drawString(String(timeString[i]), pos, tft.height() / 2 - 23, GFXFF);
          bufBefore[i] = buf[i];
          pos += (tft.textWidth(String(timeString[i])) + 1);
        }

        timeToGo = 60 * 1000 * atoi(strtok(timeStringA, ":"));
        timeToGo += 1000 * atoi(strtok(NULL, ":"));
        drawLegend("Start", "Pause", "Setzen", "", "", tft);
      }
    }
  }
}

void zaehler(TFT_eSPI tft)
{
}

void tools(TFT_eSPI tft)
{
  const char *m_menu[20];

  m_menu[0] = strdup("1. Stoppuhr");
  m_menu[1] = strdup("2. Timer");
  m_menu[2] = strdup("3. Zähler");

  switch (drawSelection(m_menu, 2, true, false, tft))
  {
  case 0:
    stoppuhr(tft);
    break;
  case 1:
    timer(tft);
    break;
  case 2:
    zaehler(tft); //drawSelection hochsetzen auf 3
    break;
  case 3:
    break;
  case 4:
    break;
  case 5:
    break;
  case 6:
    break;
  default:
    break;
  }
}
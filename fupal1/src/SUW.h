/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

static volatile unsigned long sonicStop;
static volatile unsigned long sonicStart;

static void IRAM_ATTR sonicStartM()
{
  if (sonicStart == 0)
  {
    sonicStart = micros();
  }
}

static void IRAM_ATTR sonicStopM()
{
  if (sonicStop == 0 && sonicStart > 0)
  {
    sonicStop = micros();
  }
}

void sonicSpeed(VGA3BitIscale vga)
{
  UI ui(vga);
  char buf[20];
  char bufBefore[20];
  strcpy(bufBefore, "-----------");
  int i;
  ui.drawLegend("", "", "", "", MEASUREMENT);

  unsigned long runTime;
  unsigned long sonicTime;

  int xpos;
  pinMode(12, OUTPUT);
  pinMode(26, INPUT_PULLUP);
  pinMode(5, INPUT);
  pinMode(14, INPUT);
  attachInterrupt(digitalPinToInterrupt(26), sonicStartM, FALLING);
  attachInterrupt(digitalPinToInterrupt(5), sonicStopM, RISING);

  ui.drawLegend("", "", "", "", MEASUREMENT);

while (digitalRead(14) == LOW)
  {
  }

  while (digitalRead(14) == HIGH)
  {
  }

  for (;;)
  {
    while (digitalRead(14) == HIGH)
    {
      digitalWrite(12, LOW);
    }
    digitalWrite(12, HIGH);
    sonicStop = 0;
    sonicStart = 0;
    strcpy(bufBefore, "-----------");
    vga.clear(vga.RGB(255, 255, 255));
    ui.drawLegend("", "", "", "", MEASUREMENT);
    vga.printSC(HIT_IT, 4);
    while (sonicStart == 0)
    {
      Serial.println("x");
    }
    vga.clear(vga.RGB(255, 255, 255));
    ui.drawLegend("", "", "", "", MEASUREMENT);
    while (sonicStop == 0)
    {
      runTime = micros() - sonicStart;
      if (runTime < 999999999)
      {
        sprintf(buf, "%03ld,%03ld%03lds", runTime / 1000000, (runTime / 1000) % 1000, runTime % 1000);
        //%03ld,%03ld.%03ld"
        xpos = (vga.xres / 2) - vga.font->charWidth * 22;
        vga.setCursor(xpos, (vga.yres / 2) - (vga.font->charHeight * 2));
        for (i = 0; i < 11; i++)
        {
          if (buf[i] != bufBefore[i])
          {
            vga.setTextColor(WHITE);
            vga.drawCharScale(xpos + (i * 4 * vga.font->charWidth), (vga.yres / 2) - (vga.font->charHeight * 2), 4, bufBefore[i]);
            vga.setTextColor(BLACK);
            vga.drawCharScale(xpos + (i * 4 * vga.font->charWidth), (vga.yres / 2) - (vga.font->charHeight * 2), 4, buf[i]);
            bufBefore[i] = buf[i];
          }
        }
      }
    }
    sonicTime = sonicStop - sonicStart;
    sprintf(buf, "%03ld,%03ld%03lds", sonicTime / 1000000, (sonicTime / 1000) % 1000, sonicTime % 1000);
    Serial.println(sonicTime);
    vga.clear(vga.RGB(255, 255, 255));
    //strcpy(bufBefore, "-----------");
    ui.drawLegend("", "", "", "", MEASUREMENT);
    vga.printSC(buf, 4);
    sonicTime = 0;
    sonicStop = 0;
  }
}

void selectionSUW(VGA3BitIscale vga)
{
  const char *k_menu[2];
  k_menu[0] = MENU_SUW_1;
  k_menu[1] = MENU_SUW_2; // Frequenzgenerator
  vga.setTextColor(vga.RGB(0, 0, 0));
  UI ui(vga);
  switch (ui.drawSelection(k_menu, 1, true))
  {
  case 0:
    sonicSpeed(vga);
    break;
  case 100:
      showImage(strdup("/schallg.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
      break;
  case 1:


    break;
  default:
    //
    break;
  }
}

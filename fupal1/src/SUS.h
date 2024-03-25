/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

void ecg(VGA3BitIscale vga)
{   
pinMode(26,INPUT);
pinMode(21,INPUT);
pinMode(2,INPUT);
 UI ui(vga);
ui.drawLegend("","","EKG","","");
    int y = 0;
    int lasty = 0;
    int lasti = 0;
    while(true){
        lasti = 0;
        for(int i = 0; i < 640; i++){
            y = 0;
            for(int j = 0; j < 10; j++){
                y += analogRead(26);
                delay(1);
            }
            y /= 10;
            y = 639-map(y,0,4095,200,439);
            vga.line(i,0,i,439, WHITE);
            vga.line(lasti, lasty, i, y, BLACK);
            lasti = i;
            lasty = y;
        }
    }
}

void inductionRotatingCoil(VGA3BitIscale vga)
{
  Average<float> xMean(5);
  ledcSetup(0, 200, 13);
  ledcAttachPin(26, 0);
  ledcWriteTone(0, 0);
  pinMode(2, OUTPUT); // Richtung
  boolean zoom = false;
  unsigned long int lastchange = millis();

  vga.clear(vga.RGB(255, 255, 255));
  UI ui(vga);
  ui.drawLegend("\x1E", "\x1F", Z_R_S, X1X10, STOP);
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
  vga.setCursor(4, 4);
  vga.print("U in V");
  vga.setCursor(605, 244);
  vga.print("t");
  vga.setCursor(4, 234);
  vga.print90("0");

  vga.line(20, map(1000, 2000, -2000, 20, 460), 620, map(1000, 2000, -2000, 20, 460), vga.RGB(0, 0, 0));
  vga.setCursor(4, map(1000, 2000, -2000, 20, 460) + 4);
  vga.print90("1.0");

  vga.line(20, map(-1000, 2000, -2000, 20, 460), 620, map(-1000, 2000, -2000, 20, 460), vga.RGB(0, 0, 0));
  vga.setCursor(4, map(-1000, 2000, -2000, 20, 460) + 13);
  vga.print90("-1.0");

  vga.line(20, 20, 20, 455, vga.RGB(0, 0, 0));
  int x = 21;
  int y;
  int lastyx = 240;
  char *ptr; //vorher in schleife
  int spd = 100;
  char buf[20];
  float usec;
  unsigned long int lastModeChange = 0;
  int counter = 0;
  while (true)
  {
    if (digitalRead(SW1) == LOW && (lastchange + 200) < millis() && spd < 2500)
    {
      spd += 50;

      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, X1X10, STOP);
    }
    if (digitalRead(SW2) == LOW && (lastchange + 200) < millis() && spd >= 50)
    {
      spd -= 50;
      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, X1X10, STOP);
    }

    if (digitalRead(SW4) == LOW && (millis() > (lastModeChange + 200)))
    {
      zoom = !zoom;
      lastModeChange = millis();
    }

    if (digitalRead(SW5) == LOW)
    {
      spd = 0;
      ledcWriteTone(0, spd);
      lastchange = millis();
      usec = (float)spd / 200;
      sprintf(buf, R_S_FORMAT, usec);
      ui.drawLegend("\x1E", "\x1F", buf, X1X10, STOP);
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
      if (!zoom)
      {
        y = map((int)(atof(ptr)), 2000, -2000, 20, 460);
      }
      else
      {
        y = map((int)(atof(ptr) * 10.0), 2000, -2000, 20, 460);
      }
      //xMean.push(y);  //oder halt nicht mehr mitteln
      //y = xMean.mean();

      Serial.println(message);
      if (counter > 12)
      {
        vga.line(x, y, x - 1, lastyx, vga.RGB(255, 0, 0));
        x++;
      }
      counter++;
      lastyx = y;
      if (x > 618)
      {
        x = 21;
        vga.fillRect(0, 0, vga.xres, 455, WHITE);
        vga.line(20, 240, 620, 240, vga.RGB(0, 0, 0));
        vga.line(20, 20, 20, 455, vga.RGB(0, 0, 0));

        vga.setCursor(4, 4);
        vga.print("U in V");
        vga.setCursor(605, 244);
        vga.print("t");
        vga.setCursor(4, 234);
        vga.print90("0");

        vga.line(20, map(1000, 2000, -2000, 20, 460), 620, map(1000, 2000, -2000, 20, 460), vga.RGB(0, 0, 0));
        vga.setCursor(4, map(1000, 2000, -2000, 20, 460) + 4);
        if (zoom)
        {
          vga.print90("0.1");
        }
        else
        {
          vga.print90("1.0");
        }

        vga.line(20, map(-1000, 2000, -2000, 20, 460), 620, map(-1000, 2000, -2000, 20, 460), vga.RGB(0, 0, 0));
        vga.setCursor(4, map(-1000, 2000, -2000, 20, 460) + 13);
        if (zoom)
        {
          vga.print90("-0.1");
        }
        else
        {
          vga.print90("-1.0");
        }

        counter = 0;
      }
    }
  }
}

void selectionSUS(VGA3BitIscale vga)
{
  const char *k_menu[5];
  k_menu[0] = MENU_SUS_1; // Induktion
  k_menu[1] = MENU_SUS_2; // ekg
  k_menu[2] = MENU_SUS_3; // 
  k_menu[3] = MENU_SUS_4; // 
  k_menu[4] = MENU_SUS_5;

  vga.setTextColor(vga.RGB(0, 0, 0));
  UI ui(vga);
  switch (ui.drawSelection(k_menu, 2, true))
  {
  case 0:
    inductionRotatingCoil(vga);
    break;
  case 1:
    ecg(vga);
    break;
  case 100:
    showImage(strdup("/induktion.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
    break;
  case 101:
    showImage(strdup("/sync.bmp"), vga);
      waitForButton();
      vga.clear(WHITE);
    break;
  default:
    //
    break;
  }
}

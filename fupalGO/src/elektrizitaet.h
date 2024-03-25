/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

void ekg(TFT_eSPI tft)
{
pinMode(26,INPUT);
pinMode(21,INPUT);
pinMode(2,INPUT);
    int y = 0;
    int lasty = 0;
    int lasti = 0;
    while(true){
        lasti = 0;
        for(int i = 0; i < 480; i++){
            y = 0;
            for(int j = 0; j < 10; j++){
                y += analogRead(26);
                delay(1);
            }
            y /= 10;
            y = tft.height()-map(y,0,4095,0,tft.height());
            tft.drawLine(i,0,i,tft.height(),TFT_BLACK);
            tft.drawLine(lasti, lasty, i, y, TFT_GREEN);
            lasti = i;
            lasty = y;
        }
    }
}

void induktion(TFT_eSPI tft)
{
    unsigned long int lastModeChange = 0;
    boolean zoom = false;
    Average<float> xMean(5);
    Average<float> yMean(5);
    Average<float> zMean(5);
    ledcSetup(0, 200, 13);
    ledcAttachPin(26, 0);
    ledcWriteTone(0, 0);
    pinMode(2, OUTPUT); // Richtung

    tft.fillScreen(TFT_BLACK);
    drawLegend("+", "-", "0.0 U/s", "X1/X10", "Stop", tft);
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
    tft.drawLine(20, map(1000, 2000, -2000, 10, 285), 460, map(1000, 2000, -2000, 10, 285), TFT_GREEN);
    tft.drawLine(20, map(-1000, 2000, -2000, 10, 285), 460, map(-1000, 2000, -2000, 10, 285), TFT_GREEN);
    tft.drawLine(20, 20, 20, 285, TFT_GREEN);
    tft.drawString("U in V", 0, 0);
    tft.drawString("t", 450, 149);
    drawString90("0", 0, map(0, 2000, -2000, 10, 285) + tft.textWidth("0") / 2, tft);
    drawString90("1.0", 0, map(1000, 2000, -2000, 10, 285) + tft.textWidth("1.0") / 2, tft);
    drawString90("-1.0", 0, map(-1000, 2000, -2000, 10, 285) + tft.textWidth("-1.0") / 2, tft);

    int x = 21;
    int y;
    int lastyx = 148;
    char *ptr;
    int spd = 0;
    char buf[20];
    float usec;
    unsigned long lastchange = 0;
    while (true)
    {
        //delay(1);
        delayMicroseconds(100);
        if (digitalRead(SW1) == LOW && (lastchange + 200) < millis() && spd < 2500)
        {
            if(spd == 0){
                spd = 100;
            } else {
                spd += 50;
            }
            

            ledcWriteTone(0, spd);
            lastchange = millis();
            usec = (float)spd / 200;
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "X1/X10", "Stop", tft);
        }
        if (digitalRead(SW2) == LOW && (lastchange + 200) < millis() && spd > 0)
        {
            if(spd == 100){
                spd = 0;
            } else {
                spd -= 50;
            }
            ledcWriteTone(0, spd);
            lastchange = millis();
            usec = (float)spd / 200;
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "X1/X10", "Stop", tft);
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
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "X1/X10", "Stop", tft);
        }

        //Serial.println(spd);

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
            if (zoom)
            {
                y = map((int)(atof(ptr) * 10.0), 2000, -2000, 10, 285);
            }
            else
            {
                y = map((int)(atof(ptr)), 2000, -2000, 10, 285);
            }

            xMean.push(y);
            y = xMean.mean();
            tft.drawLine(x, y, x - 1, lastyx, TFT_RED);
            lastyx = y;
            x++;
            if (x > 458)
            {
                x = 21;
                tft.fillRect(0, 0, tft.width(), 285, TFT_BLACK);
                tft.drawLine(20, 147, 460, 147, TFT_GREEN);
                tft.drawLine(20, map(1000, 2000, -2000, 10, 285), 460, map(1000, 2000, -2000, 10, 285), TFT_GREEN);
                tft.drawLine(20, map(-1000, 2000, -2000, 10, 285), 460, map(-1000, 2000, -2000, 10, 285), TFT_GREEN);
                tft.drawLine(20, 20, 20, 285, TFT_GREEN);
                tft.drawString("U in V", 0, 0);
                tft.drawString("t", 450, 149);
                drawString90("0", 0, map(0, 2000, -2000, 10, 285) + tft.textWidth("0") / 2, tft);
                if (!zoom)
                {
                    drawString90("1.0", 0, map(1000, 2000, -2000, 10, 285) + tft.textWidth("1.0") / 2, tft);
                    drawString90("-1.0", 0, map(-1000, 2000, -2000, 10, 285) + tft.textWidth("-1.0") / 2, tft);
                }
                else
                {
                    drawString90("0.1", 0, map(1000, 2000, -2000, 10, 285) + tft.textWidth("0.1") / 2, tft);
                    drawString90("-0.1", 0, map(-1000, 2000, -2000, 10, 285) + tft.textWidth("-0.1") / 2, tft);
                }
//delay(500);
                int flushed = 0;
                while (flushed < 10)
                {
                    while (radio.available())
                    {
                        radio.read(message, sizeof(message));
                        ptr = strtok(message, ";");
            if (zoom)
            {
                lastyx = map((int)(atof(ptr) * 10.0), 2000, -2000, 10, 285);
            }
            else
            {
                lastyx = map((int)(atof(ptr)), 2000, -2000, 10, 285);
            }
            xMean.push(lastyx);
            lastyx = xMean.mean();
                        flushed++;
                    }
                }
                //lastyx=0;
                //
            }
        }
    }
}

void elektrizitaet(TFT_eSPI tft)
{
    const char *m_menu[20];

    m_menu[0] = strdup("1. Rotierende Spule im Magnetfeld");
    m_menu[1] = strdup("2. Elektrokardiogramm");

    switch (drawSelection(m_menu, 2, true, true, tft))
    {
    case 0:
        induktion(tft);
        break;
    case 1:
        ekg(tft);
        break;
    case 100:
        drawJpeg("/induktion.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 101:
        drawJpeg("/ekg.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;

        
    default:
        break;
    }
}
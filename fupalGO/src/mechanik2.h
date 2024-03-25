/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

//Periode Fadenpendel 1LS
void periodeFadenpendel(TFT_eSPI tft)
{
    unsigned long t1;
    unsigned long middle1;
    unsigned long middle2;
    pinMode(26, INPUT);
    Table table(4, 11, false);
    char *cols[10];
    cols[0] = strdup("Nr.");
    cols[1] = strdup("t in ms");
    cols[2] = strdup("Nr.");
    cols[3] = strdup("t in ms");

    table.setColumns(cols);
    table.draw(tft);
    drawLegend("", "", "", "", "Reset", tft);
    while(true){
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
                if (digitalRead(SW5) == LOW)
                {
                    table.clean();
                    table.draw(tft);
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
                    table.draw(tft);
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
                    table.draw(tft);
                    i = 0;
                    k = 0;
                }
            }
            middle2 = (micros() + t1) / 2;
            table.setValueDraw((k * 2), i, (i + 1) + k * 11, tft);
            table.setValueDraw((k * 2) + 1, i, (middle2 - middle1) / 1000.0, tft);
        }
    }

    while (digitalRead(SW5) == HIGH){}
                
                    table.clean();
                    table.draw(tft);
                    middle2 = 0;
                

    }
}

//Periode Federpendel 1LS
void periodeFederpendel(TFT_eSPI tft)
{
    unsigned long t1;
    unsigned long middle1;
    unsigned long middle2 = 0;
    pinMode(26, INPUT);
    Table table(4, 11, false);
    char *cols[10];
    cols[0] = strdup("Nr.");
    cols[1] = strdup("t in ms");
    cols[2] = strdup("Nr.");
    cols[3] = strdup("t in ms");
    table.setColumns(cols);
    table.draw(tft);
    drawLegend("", "", "", "", "Reset", tft);
    while(true){
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
                    table.draw(tft);
                    i = 0;
                    k = 0;
                    middle2 = 0;
                }
            }
            middle1 = (micros() + t1) / 2;
            if (middle2 != 0)
            {
                table.setValueDraw((k * 2), i, (i + 1) + k * 11, tft);
                table.setValueDraw((k * 2) + 1, i, (middle1 - middle2) / 1000.0, tft);
            }
            else
            {
                i--;
            }
            middle2 = middle1;
        }
    }
    while (digitalRead(SW5) == HIGH){}
                
                    table.clean();
                    table.draw(tft);
                    middle2 = 0;
                
    }
}

void motorSynchronisiertPendel(TFT_eSPI tft)
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
    while (true)
    {
        tft.fillScreen(TFT_BLACK);
        printMessage("Bestimmung der Periodendauer", tft);
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

        tft.fillScreen(TFT_BLACK);
        sprintf(buf, "%.2f ms", 0.001 * period);
        printMessage(buf, tft);
        drawLegend("", "", "", "", "Reset", tft);
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

void radialkraftWert(TFT_eSPI tft)
{

    Average<float> aMean(50);
    ledcSetup(0, 200, 13);
    ledcAttachPin(26, 0);
    ledcWriteTone(0, 0);
    pinMode(2, OUTPUT); // Richtung
    tft.fillScreen(TFT_BLACK);
    drawLegend("+", "-", "", "Start", "Stop", tft);
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
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "Start", "Stop", tft);
        }
        if (digitalRead(SW2) == LOW && (lastchange + 200) < millis() && spd >= 300)
        {
            spd -= 100;
            ledcWriteTone(0, spd);
            lastchange = millis();
            usec = (float)spd / 200;
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "Start", "Stop", tft);
        }

        if (digitalRead(SW4) == LOW)
        {
            spd = 200;
            ledcWriteTone(0, spd);
            lastchange = millis();
            usec = (float)spd / 200;
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "Start", "Stop", tft);
        }
        if (digitalRead(SW5) == LOW)
        {
            spd = 0;
            ledcWriteTone(0, spd);
            lastchange = millis();
            usec = (float)spd / 200;
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("-", "+", buf, "Start", "Stop", tft);
        }

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
            //ptr = strtok(NULL, ";");
            y = (atof(ptr) * -1.0);
            aMean.push(y);
            printY = aMean.mean();
            if ((printY > lastPrintY + 0.01 || printY < lastPrintY - 0.01) && millis() > lastOutput + 300)
            {
                tft.setTextColor(TFT_BLACK);
                sprintf(output, "%.1f m/s²", lastPrintY);
                printMessage(output, tft);
                tft.setTextColor(TFT_GREEN);
                sprintf(output, "%.1f m/s²", printY);
                printMessage(output, tft);
                lastPrintY = printY;
                lastOutput = millis();
            }
            delay(10);
        }
    }
}

void radialkraftGraph(TFT_eSPI tft)
{
    Average<float> xMean(5);
    Average<float> yMean(5);
    Average<float> zMean(5);
    ledcSetup(0, 200, 13);
    ledcAttachPin(26, 0);
    ledcWriteTone(0, 0);
    pinMode(2, OUTPUT); // Richtung

    tft.fillScreen(TFT_BLACK);
    drawLegend("+", "-", "0.0 U/s", "Start", "Stop", tft);
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
    tft.drawLine(20, map(5000, 20000, -20000, 10, 285), 460, map(5000, 20000, -20000, 10, 285), TFT_GREEN);
    tft.drawLine(20, map(-5000, 20000, -20000, 10, 285), 460, map(-5000, 20000, -20000, 10, 285), TFT_GREEN);
    tft.drawLine(20, map(10000, 20000, -20000, 10, 285), 460, map(10000, 20000, -20000, 10, 285), TFT_GREEN);
    tft.drawLine(20, map(-10000, 20000, -20000, 10, 285), 460, map(-10000, 20000, -20000, 10, 285), TFT_GREEN);
    tft.drawLine(20, map(15000, 20000, -20000, 10, 285), 460, map(15000, 20000, -20000, 10, 285), TFT_GREEN);
    tft.drawLine(20, map(-15000, 20000, -20000, 10, 285), 460, map(-15000, 20000, -20000, 10, 285), TFT_GREEN);
    tft.drawLine(20, 20, 20, 285, TFT_GREEN);
    tft.drawString("a in m/s²", 0, 0);
    tft.drawString("t", 450, 149);
    drawString90("0", 0, map(0, 20000, -20000, 10, 285) + tft.textWidth("0") / 2, tft);
    drawString90("100", 0, map(10000, 20000, -20000, 10, 285) + tft.textWidth("100") / 2, tft);
    drawString90("-100", 0, map(-10000, 20000, -20000, 10, 285) + tft.textWidth("-100") / 2, tft);

    int x = 21;
    int y;
    int lastyx = 148;
    int lastyy = 148;
    int lastyz = 148;
    char *ptr;
    int spd = 100;
    char buf[20];
    float usec;
    unsigned long lastchange = 0;
    while (true)
    {
        //delay(1);
        delayMicroseconds(100);
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
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "Start", "Stop", tft);
        }
        if (digitalRead(SW2) == LOW && (lastchange + 200) < millis() && spd >= 300)
        {
            spd -= 100;
            ledcWriteTone(0, spd);
            lastchange = millis();
            usec = (float)spd / 200;
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "Start", "Stop", tft);
        }

        if (digitalRead(SW4) == LOW)
        {
            spd = 200;
            ledcWriteTone(0, spd);
            lastchange = millis();
            usec = (float)spd / 200;
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "Start", "Stop", tft);
        }
        if (digitalRead(SW5) == LOW)
        {
            spd = 0;
            ledcWriteTone(0, spd);
            lastchange = millis();
            usec = (float)spd / 200;
            sprintf(buf, "%.1f U/s", usec);
            drawLegend("+", "-", buf, "Start", "Stop", tft);
        }

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
            y = map((int)(atof(ptr) * -100), 20000, -20000, 10, 285);
            xMean.push(y);
            y = xMean.mean();
            tft.drawLine(x, y, x - 1, lastyx, TFT_RED);
            lastyx = y;
            ptr = strtok(NULL, ";");
            y = map((int)(atof(ptr) * 100), 20000, -20000, 10, 285);
            yMean.push(y);
            y = yMean.mean();
            tft.drawLine(x, y, x - 1, lastyy, TFT_YELLOW);
            lastyy = y;
            ptr = strtok(NULL, ";");
            y = map((int)(atof(ptr) * 100), 20000, -20000, 10, 285);
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
                tft.drawLine(20, map(5000, 20000, -20000, 10, 285), 460, map(5000, 20000, -20000, 10, 285), TFT_GREEN);
                tft.drawLine(20, map(-5000, 20000, -20000, 10, 285), 460, map(-5000, 20000, -20000, 10, 285), TFT_GREEN);
                tft.drawLine(20, map(10000, 20000, -20000, 10, 285), 460, map(10000, 20000, -20000, 10, 285), TFT_GREEN);
                tft.drawLine(20, map(-10000, 20000, -20000, 10, 285), 460, map(-10000, 20000, -20000, 10, 285), TFT_GREEN);
                tft.drawLine(20, map(15000, 20000, -20000, 10, 285), 460, map(15000, 20000, -20000, 10, 285), TFT_GREEN);
                tft.drawLine(20, map(-15000, 20000, -20000, 10, 285), 460, map(-15000, 20000, -20000, 10, 285), TFT_GREEN);
                tft.drawLine(20, 20, 20, 285, TFT_GREEN);
                tft.drawString("a in m/s²", 0, 0);
                tft.drawString("t", 450, 149);
                drawString90("0", 0, map(0, 20000, -20000, 10, 285) + tft.textWidth("0") / 2, tft);
                drawString90("100", 0, map(10000, 20000, -20000, 10, 285) + tft.textWidth("100") / 2, tft);
                drawString90("-100", 0, map(-10000, 20000, -20000, 10, 285) + tft.textWidth("-100") / 2, tft);
            }
            delay(5);
        }
    }
}

void mechanik2(TFT_eSPI tft)
{

    const char *m_menu[20];

    m_menu[0] = strdup("1. Radialbeschleunigung (Graph)");
    m_menu[1] = strdup("2. Radialbeschleunigung (Wert)");
    m_menu[2] = strdup("3. Periode eines Fadenpendels");
    m_menu[3] = strdup("4. Periode eines Federpendels");
    m_menu[4] = strdup("6. Synch. eines Motors auf ein Pendel");

    switch (drawSelection(m_menu, 5, true, true, tft))
    {
    case 0:
        radialkraftGraph(tft);
        break;
    case 1:
        radialkraftWert(tft);
        break;
    case 2:
        periodeFadenpendel(tft);
        break;
    case 3:
        periodeFederpendel(tft);
        break;
    case 4:
        motorSynchronisiertPendel(tft);
        break;
    case 100:
        drawJpeg("/radialbesch.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 101:
        drawJpeg("/radialbesch.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 102:
        drawJpeg("/periode.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 103:
        drawJpeg("/periode.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 104:
        drawJpeg("/synchro.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  
    default:
        break;
    }
}
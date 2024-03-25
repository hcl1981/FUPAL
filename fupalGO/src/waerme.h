/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

//1 Sensor
static float readTemp1;
static boolean tempReady1;

static void TemperaturTask1(void *pvParameters)
{
    OneWire oneWire(26);
    DallasTemperature tempSensor(&oneWire);
    tempSensor.begin();

    while (true)
    {
        tempSensor.requestTemperaturesByIndex(0);
        readTemp1 = tempSensor.getTempCByIndex(0);
        tempReady1 = true;
        delay(2000);
    }
}

void aufzeichnung1temperatur(TFT_eSPI tft)
{
    tft.setTextDatum(MC_DATUM);

    xTaskCreatePinnedToCore(TemperaturTask1, "Task1", 10000, NULL, 1, &Task1, 0);

    Table table(2, 11, true);
    char *cols[10];
    char buf[15];
    cols[0] = strdup("t in s");
    cols[1] = strdup("T in °C");

    table.setColumns(cols);
    table.draw(tft);
    drawLegend("10 Sek.", "1 Min.", "5 Min.", "", "Reset", tft);
    boolean selected = false;
    int interval = 0;
    float lastTemp;

    while (true)
    {
        selected = false;
        lastTemp = 0;
        while (!selected)
        {
            if (digitalRead(SW1) == LOW)
            {
                interval = 10;
                selected = true;
            }
            if (digitalRead(SW2) == LOW)
            {
                interval = 60;
                selected = true;
            }
            if (digitalRead(SW3) == LOW)
            {
                interval = 300;
                selected = true;
            }
            if (readTemp1 != lastTemp)
            {
                sprintf(buf, "%.2f°C", readTemp1);

                tft.fillRect(tft.width() * 0.75 - (tft.textWidth(buf) / 2 + 20), tft.height() / 2 - 40, tft.textWidth(buf) + 40, 40, TFT_BLACK);
                tft.drawRect(tft.width() * 0.75 - (tft.textWidth(buf) / 2 + 20), tft.height() / 2 - 40, tft.textWidth(buf) + 40, 40, TFT_GREEN);
                tft.drawRect(tft.width() * 0.75 - (tft.textWidth(buf) / 2 + 23), tft.height() / 2 - 43, tft.textWidth(buf) + 46, 46, TFT_GREEN);
                tft.drawString(buf, tft.width() * 0.75, tft.height() / 2 - 23, GFXFF);

                lastTemp = readTemp1;
            }
        }
        unsigned long starttime = millis();

        for (int i = 0; i < 11; i++)
        {
            table.setValueDraw(0, i, i * interval, tft);
            table.setValueDraw(1, i, readTemp1, tft);

            while (millis() < (starttime + (i + 1) * interval * 1000) && selected)
            {
                if (readTemp1 != lastTemp)
                {
                    sprintf(buf, "%.2f°C", readTemp1);
                    tft.fillRect(tft.width() * 0.75 - (tft.textWidth(buf) / 2 + 20), tft.height() / 2 - 40, tft.textWidth(buf) + 40, 40, TFT_BLACK);
                    tft.drawRect(tft.width() * 0.75 - (tft.textWidth(buf) / 2 + 20), tft.height() / 2 - 40, tft.textWidth(buf) + 40, 40, TFT_GREEN);
                    tft.drawRect(tft.width() * 0.75 - (tft.textWidth(buf) / 2 + 23), tft.height() / 2 - 43, tft.textWidth(buf) + 46, 46, TFT_GREEN);
                    tft.drawString(buf, tft.width() * 0.75, tft.height() / 2 - 23, GFXFF);
                    lastTemp = readTemp1;
                }
                if (digitalRead(SW5) == LOW)
                {
                    i = 100;
                    selected = false;
                    table.clean();
                    table.draw(tft);
                }
            }
        }
        while (digitalRead(SW5) == HIGH)
        {
        }

        selected = false;
        table.clean();
        table.draw(tft);
    }
}

//2 Sensoren
static float readTemp2;
static boolean tempReady2;

static void TemperaturTask2(void *pvParameters)
{
    OneWire oneWire(5);
    DallasTemperature tempSensor2(&oneWire);
    tempSensor2.begin();

    while (true)
    {
        tempSensor2.requestTemperaturesByIndex(0);
        readTemp2 = tempSensor2.getTempCByIndex(0);
        tempReady2 = true;
        delay(2000);
    }
}

void aufzeichnung2temperaturen(TFT_eSPI tft)
{
    tft.setTextDatum(MC_DATUM);
    xTaskCreatePinnedToCore(TemperaturTask1, "Task1", 10000, NULL, 1, &Task1, 0);
    xTaskCreatePinnedToCore(TemperaturTask2, "Task2", 10000, NULL, 1, &Task2, 0);

    Table table(3, 11, true);
    char *cols[10];
    char buf1[15];
    char buf2[15];
    cols[0] = strdup("t in s");
    cols[1] = strdup("t1 in °C");
    cols[2] = strdup("t2 in °C");

    table.setColumns(cols);
    table.draw(tft);
    drawLegend("10 Sek.", "1 Min.", "5 Min.", "", "Reset", tft);
    boolean selected = false;
    int interval = 0;
    float lastTemp1;
    float lastTemp2;

    while (true)
    {
        selected = false;
        lastTemp1 = 0;
        lastTemp2 = 0;
        while (!selected)
        {
            if (digitalRead(SW1) == LOW)
            {
                interval = 10;
                selected = true;
            }
            if (digitalRead(SW2) == LOW)
            {
                interval = 60;
                selected = true;
            }
            if (digitalRead(SW3) == LOW)
            {
                interval = 300;
                selected = true;
            }
            if (readTemp1 != lastTemp1 || readTemp2 != lastTemp2)
            {

                sprintf(buf1, "%.2f°C", readTemp1);
                sprintf(buf2, "%.2f°C", readTemp2);

                int width = max(tft.textWidth(buf1), tft.textWidth(buf2));

                tft.fillRect(tft.width() * 0.75 - (width / 2 + 23), tft.height() / 2 - 63, width + 46, 66, TFT_BLACK);
                tft.drawRect(tft.width() * 0.75 - (width / 2 + 20), tft.height() / 2 - 60, width + 40, 60, TFT_GREEN);
                tft.drawRect(tft.width() * 0.75 - (width / 2 + 23), tft.height() / 2 - 63, width + 46, 66, TFT_GREEN);
                tft.drawString(buf1, tft.width() * 0.75, tft.height() / 2 - 43, GFXFF);
                tft.drawString(buf2, tft.width() * 0.75, tft.height() / 2 - 23, GFXFF);

                lastTemp1 = readTemp1;
                lastTemp2 = readTemp2;
            }
        }
        unsigned long starttime = millis();

        for (int i = 0; i < 11; i++)
        {
            table.setValueDraw(0, i, i * interval, tft);
            table.setValueDraw(1, i, readTemp1, tft);
            table.setValueDraw(2, i, readTemp2, tft);

            while (millis() < (starttime + (i + 1) * interval * 1000) && selected)
            {
            if (readTemp1 != lastTemp1 || readTemp2 != lastTemp2)
            {

                sprintf(buf1, "%.2f°C", readTemp1);
                sprintf(buf2, "%.2f°C", readTemp2);

                int width = max(tft.textWidth(buf1), tft.textWidth(buf2));

                tft.fillRect(tft.width() * 0.75 - (width / 2 + 23), tft.height() / 2 - 63, width + 46, 66, TFT_BLACK);
                tft.drawRect(tft.width() * 0.75 - (width / 2 + 20), tft.height() / 2 - 60, width + 40, 60, TFT_GREEN);
                tft.drawRect(tft.width() * 0.75 - (width / 2 + 23), tft.height() / 2 - 63, width + 46, 66, TFT_GREEN);
                tft.drawString(buf1, tft.width() * 0.75, tft.height() / 2 - 43, GFXFF);
                tft.drawString(buf2, tft.width() * 0.75, tft.height() / 2 - 23, GFXFF);

                lastTemp1 = readTemp1;
                lastTemp2 = readTemp2;
            }
                if (digitalRead(SW5) == LOW)
                {
                    i = 100;
                    selected = false;
                    table.clean();
                    table.draw(tft);
                }
            }
        }
        while (digitalRead(SW5) == HIGH)
        {
        }
        selected = false;
        table.clean();
        table.draw(tft);
    }
}

void messung1temperatur(TFT_eSPI tft)
{
    tft.setTextDatum(MC_DATUM);
    OneWire oneWire(26);
    static char readTemp[20];
    DallasTemperature tempSensor(&oneWire);
    tempSensor.begin();

    delay(200);

    while (true)
    {
        tempSensor.requestTemperaturesByIndex(0);
        sprintf(readTemp, "%.2f°C", tempSensor.getTempCByIndex(0));

        tft.fillRect(tft.width() / 2 - (tft.textWidth(readTemp) / 2 + 20), tft.height() / 2 - 40, tft.textWidth(readTemp) + 40, 40, TFT_BLACK);
        tft.drawRect(tft.width() / 2 - (tft.textWidth(readTemp) / 2 + 20), tft.height() / 2 - 40, tft.textWidth(readTemp) + 40, 40, TFT_GREEN);
        tft.drawRect(tft.width() / 2 - (tft.textWidth(readTemp) / 2 + 23), tft.height() / 2 - 43, tft.textWidth(readTemp) + 46, 46, TFT_GREEN);
        tft.drawString(readTemp, tft.width() / 2, tft.height() / 2 - 23, GFXFF);
        delay(2000);
    }
}

void messung2temperaturen(TFT_eSPI tft)
{
    tft.setTextDatum(MC_DATUM);
    OneWire oneWire1(26);
    OneWire oneWire2(5);
    static char readTemp1[20];
    static char readTemp2[20];
    DallasTemperature tempSensor1(&oneWire1);
    DallasTemperature tempSensor2(&oneWire2);
    tempSensor1.begin();
    tempSensor2.begin();
    int width;

    delay(200);

    while (true)
    {
        tempSensor1.requestTemperaturesByIndex(0);
        sprintf(readTemp1, "%.2f°C", tempSensor1.getTempCByIndex(0));

        tempSensor2.requestTemperaturesByIndex(0);
        sprintf(readTemp2, "%.2f°C", tempSensor2.getTempCByIndex(0));

        width = max(tft.textWidth(readTemp1), tft.textWidth(readTemp2));

        tft.fillRect(0, tft.height() / 2 - 60, tft.width() + 40, 66, TFT_BLACK);
        tft.drawRect(tft.width() / 2 - (width / 2 + 20), tft.height() / 2 - 60, width + 40, 60, TFT_GREEN);
        tft.drawRect(tft.width() / 2 - (width / 2 + 23), tft.height() / 2 - 63, width + 46, 66, TFT_GREEN);
        tft.drawString(readTemp1, tft.width() / 2, tft.height() / 2 - 43, GFXFF);
        tft.drawString(readTemp2, tft.width() / 2, tft.height() / 2 - 23, GFXFF);

        delay(2000);
    }
}

void druckwaerme(TFT_eSPI tft)
{

    drawLegend("", "", "", "", "Messung", tft);
    tft.setTextDatum(MC_DATUM);
    Adafruit_BMP280 bmp;

    if (!bmp.begin())
    {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
        while (1)
            ;
    }

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X4,     /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X4,       /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    float tT;
    float tP;
    char buf1[12];
    char buf2[12];
    delay(200);
    while (true)
    {
        while (digitalRead(SW5) == HIGH)
        {
            tT = bmp.readTemperature();
            tP = bmp.readPressure();
        }

        tT = bmp.readTemperature();
        tP = bmp.readPressure();

        sprintf(buf1, "%.2f°C", tT);
        sprintf(buf2, "%.2f Pa", tP);

        tft.fillRect(0, tft.height() / 2 - 60, tft.width() + 40, 66, TFT_BLACK);
        tft.drawRect(tft.width() / 2 - (tft.textWidth(buf2) / 2 + 20), tft.height() / 2 - 60, tft.textWidth(buf2) + 40, 60, TFT_GREEN);
        tft.drawRect(tft.width() / 2 - (tft.textWidth(buf2) / 2 + 23), tft.height() / 2 - 63, tft.textWidth(buf2) + 46, 66, TFT_GREEN);
        tft.drawString(buf1, tft.width() / 2, tft.height() / 2 - 43, GFXFF);
        tft.drawString(buf2, tft.width() / 2, tft.height() / 2 - 23, GFXFF);
        delay(100);
    }
}

void waerme(TFT_eSPI tft)
{

    const char *m_menu[20];

    m_menu[0] = strdup("1. Messung einer Temperatur");
    m_menu[1] = strdup("2. Messung zweier Temperaturen");
    m_menu[2] = strdup("3. Aufzeichnung einer Temperatur");
    m_menu[3] = strdup("4. Aufzeichnung zweier Temperaturen");
    m_menu[4] = strdup("5. Messung von Druck und Temperatur");

    switch (drawSelection(m_menu, 5, true, true, tft))
    {
    case 0:
        messung1temperatur(tft);
        break;
    case 1:
        messung2temperaturen(tft);
        break;
    case 2:
        aufzeichnung1temperatur(tft);
        break;
    case 3:
        aufzeichnung2temperaturen(tft);
        break;
    case 4:
        druckwaerme(tft);
        break;
    case 100:
        drawJpeg("/temp.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 101:
        drawJpeg("/temp.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 102:
        drawJpeg("/temp.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 103:
        drawJpeg("/temp.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    case 104:
        drawJpeg("/nv.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  
    default:
        break;
    }
}
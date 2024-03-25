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
    if (sonicStop == 0 && sonicStart > 0) // && sonicStart > 0?
    {
        sonicStop = micros();
    }
}

void schallgeschwindigkeit(TFT_eSPI tft)
{

    char buf[20];
    char bufBefore[20];
    strcpy(bufBefore, "-----------");
    drawLegend("", "", "", "", "Messung", tft);

    unsigned long runTime;
    unsigned long sonicTime;

    pinMode(12, OUTPUT);
    pinMode(26, INPUT_PULLUP);
    pinMode(5, INPUT);
    pinMode(14, INPUT);
    attachInterrupt(digitalPinToInterrupt(26), sonicStartM, FALLING);
    attachInterrupt(digitalPinToInterrupt(5), sonicStopM, RISING);

    for (;;)
    {
        tft.setTextDatum(MC_DATUM);
        while (digitalRead(14) == HIGH)
        {
            digitalWrite(12, LOW);
        }
        digitalWrite(12, HIGH);
        sonicStop = 0;
        sonicStart = 0;
        strcpy(bufBefore, "-----------");
        tft.fillScreen(TFT_BLACK);
        drawLegend("", "", "", "", "Messung", tft);
        printMessage("Hau drauf!", tft);
        while (sonicStart == 0)
        {
            //Serial.println("x");
        }
        //tft.fillScreen(TFT_BLACK);
        //drawLegend("", "", "", "", "Messung", tft);
        tft.setTextColor(TFT_BLACK, TFT_BLACK);
        printMessage("Hau drauf!", tft);
        clearFrame("Hau drauf!", tft);
        
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setTextDatum(ML_DATUM);
        while (sonicStop == 0)
        {
            runTime = micros() - sonicStart;
            if (runTime < 999999999)
            {
                sprintf(buf, "%03ld:%03ld.%03ld", runTime / 1000000, (runTime / 1000) % 1000, runTime % 1000);
                tft.setCursor(tft.width() / 2 - tft.textWidth(buf) / 2, tft.height() / 2 - 23);
                int pos = tft.width() / 2 - tft.textWidth(buf) / 2;
                for (int i = 0; i < 11; i++)
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
                    pos += tft.textWidth(String(buf[i]));
                }
            }
        }
        sonicTime = sonicStop - sonicStart;
        sprintf(buf, "%03ld:%03ld.%03ld", sonicTime / 1000000, (sonicTime / 1000) % 1000, sonicTime % 1000);
        Serial.println(sonicTime);
        
        //tft.fillScreen(TFT_BLACK);
        strcpy(bufBefore, "-----------");
        //drawLegend("", "", "", "", "Messung", tft);
        printMessage(buf, tft);
        tft.setTextColor(TFT_GREEN);
        //vga.printSC(buf, 4);
        sonicTime = 0;
        sonicStop = 0;
    }
}

void akustik(TFT_eSPI tft)
{
    const char *m_menu[20];

    //Tongenerator"
    m_menu[0] = strdup("1. Schallgeschwindigkeit");

    switch (drawSelection(m_menu, 1, true, true, tft))
    {
    case 0:
        schallgeschwindigkeit(tft);
        break;
    case 100:
        drawJpeg("/schallgeschw.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
    default:
        break;
    }
}
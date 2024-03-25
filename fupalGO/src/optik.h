/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

word convertRGB( byte R, byte G, byte B)
{
  return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );
}

void additiveFarbmischung(TFT_eSPI tft)
{

  tft.setTextDatum(BC_DATUM);
  tft.fillRect(0, tft.height() - 25, tft.width(), 25, TFT_BLACK);
  tft.drawLine(0, tft.height() - 25, tft.width(), tft.height() - 25, TFT_GREEN);
  tft.drawLine(1 * tft.width() / 5, tft.height() - 25, 1 * tft.width() / 5, tft.height(), TFT_GREEN);
  tft.drawLine(2 * tft.width() / 5, tft.height() - 25, 2 * tft.width() / 5, tft.height(), TFT_GREEN);
  tft.drawLine(3 * tft.width() / 5, tft.height() - 25, 3 * tft.width() / 5, tft.height(), TFT_GREEN);
  tft.drawLine(4 * tft.width() / 5, tft.height() - 25, 4 * tft.width() / 5, tft.height(), TFT_GREEN);
  tft.setTextColor(TFT_RED);
  tft.drawString("Rot", 0.5 * tft.width() / 5, tft.height() - 3, GFXFF);
  tft.setTextColor(TFT_GREEN);
  tft.drawString("Grün", 1.5 * tft.width() / 5, tft.height() - 3, GFXFF);
  tft.setTextColor(TFT_BLUE);
  tft.drawString("Blau", 2.5 * tft.width() / 5, tft.height() - 3, GFXFF);
  tft.setTextColor(TFT_GREEN);
  tft.drawString("+", 3.5 * tft.width() / 5, tft.height() - 3, GFXFF);
  tft.drawString("-", 4.5 * tft.width() / 5, tft.height() - 3, GFXFF);

    byte r = 127;
    byte g = 127;
    byte b = 127;
    int current = 0;

    tft.drawRect(30,(tft.height() - 310),57,266,TFT_WHITE);

    for(int i = 0; i < 256; i++){
        tft.drawLine(50,(tft.height() - 50)-i,80,(tft.height() - 50)-i,convertRGB(i,0,0));
        
        tft.drawLine(120,(tft.height() - 50)-i,150,(tft.height() - 50)-i,convertRGB(0,i,0));
        
        tft.drawLine(190,(tft.height() - 50)-i,220,(tft.height() - 50)-i,convertRGB(0,0,i));

    }

                tft.drawLine(35,(tft.height() - 50)-r,45,(tft.height() - 50)-r,TFT_WHITE);       
                tft.drawLine(105,(tft.height() - 50)-g,115,(tft.height() - 50)-g,TFT_WHITE);
                tft.drawLine(175,(tft.height() - 50)-b,185,(tft.height() - 50)-b,TFT_WHITE);


    while(true){
        if(digitalRead(SW1)==LOW){
            current = 0;
            tft.drawRect(30,(tft.height() - 310),57,266,TFT_WHITE);
            tft.drawRect(100,(tft.height() - 310),57,266,TFT_BLACK);
            tft.drawRect(170,(tft.height() - 310),57,266,TFT_BLACK);
        }
        if(digitalRead(SW2)==LOW){
            current = 1;
            tft.drawRect(30,(tft.height() - 310),57,266,TFT_BLACK);
            tft.drawRect(100,(tft.height() - 310),57,266,TFT_WHITE);
            tft.drawRect(170,(tft.height() - 310),57,266,TFT_BLACK);
        }
        if(digitalRead(SW3)==LOW){
            current = 2;
            tft.drawRect(30,(tft.height() - 310),57,266,TFT_BLACK);
            tft.drawRect(100,(tft.height() - 310),57,266,TFT_BLACK);
            tft.drawRect(170,(tft.height() - 310),57,266,TFT_WHITE);
        }
        if(digitalRead(SW4)==LOW){//+
            if(current == 0 && r < 255){
                tft.drawLine(35,(tft.height() - 50)-r,45,(tft.height() - 50)-r,TFT_BLACK);
                r++;
                tft.drawLine(35,(tft.height() - 50)-r,45,(tft.height() - 50)-r,TFT_WHITE);
            }
            if(current == 1 && g < 255){
                tft.drawLine(105,(tft.height() - 50)-g,115,(tft.height() - 50)-g,TFT_BLACK);
                g++;
                tft.drawLine(105,(tft.height() - 50)-g,115,(tft.height() - 50)-g,TFT_WHITE);
            }
            if(current == 2 && b < 255){
                tft.drawLine(175,(tft.height() - 50)-b,185,(tft.height() - 50)-b,TFT_BLACK);
                b++;
                tft.drawLine(175,(tft.height() - 50)-b,185,(tft.height() - 50)-b,TFT_WHITE);
            }
        }
        if(digitalRead(SW5)==LOW){
            if(current == 0 && r > 0){
                tft.drawLine(35,(tft.height() - 50)-r,45,(tft.height() - 50)-r,TFT_BLACK);
                r--;
                tft.drawLine(35,(tft.height() - 50)-r,45,(tft.height() - 50)-r,TFT_WHITE);
            }
            if(current == 1 && g > 0){
                tft.drawLine(105,(tft.height() - 50)-g,115,(tft.height() - 50)-g,TFT_BLACK);
                g--;
                tft.drawLine(105,(tft.height() - 50)-g,115,(tft.height() - 50)-g,TFT_WHITE);
            }
            if(current == 2 && b > 0){
                tft.drawLine(175,(tft.height() - 50)-b,185,(tft.height() - 50)-b,TFT_BLACK);
                b--;
                tft.drawLine(175,(tft.height() - 50)-b,185,(tft.height() - 50)-b,TFT_WHITE);
            }

        }
        tft.fillRect(300,(tft.height() - 125)/2,100,100,convertRGB(r,g,b));
    }

}

void optik(TFT_eSPI tft)
{

    const char *m_menu[20];

    m_menu[0] = strdup("1. Additive Farbmischung");
    m_menu[1] = strdup("2. ");
    m_menu[2] = strdup("3. ");
    m_menu[3] = strdup("4. ");
    m_menu[4] = strdup("5. ");
    m_menu[5] = strdup("6. ");
    m_menu[6] = strdup("7. ");

    switch (drawSelection(m_menu, 1, true, true, tft))
    {
    case 0:
        additiveFarbmischung(tft);
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 100:
        drawJpeg("/farbmischer.jpg", 0, 0, tft);
        waitForButton();
        tft.fillScreen(TFT_BLACK);
        break;
  
    default:
        break;
    }
}
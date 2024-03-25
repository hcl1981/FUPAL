/*
	Author: Jan Schulz 2019-2021
	Creative Commons Attribution ShareAlike 4.0
	https://creativecommons.org/licenses/by-sa/4.0/

  partly based on bitlunis ESP32Lib: https://github.com/bitluni/ESP32Lib
	
	For further details check out: 
		https://513kid.de
*/

#include <ESP32Lib.h>

using namespace std;

class VGA3BitIscale : public VGA3BitI
{
  public:
    void drawCharScale(int x, int y, int scale, int ch)
    {
      if (!font)
        return;
      if (!font->valid(ch))
        return;
      const unsigned char *pix = &font->pixels[font->charWidth * font->charHeight * (ch - font->firstChar)];
      for (int py = 0; py < font->charHeight; py++)
        for (int px = 0; px < font->charWidth; px++)
          if (*(pix++)) {
            for (int i = px * scale; i < px * scale + scale; i++) {
              for (int j = py * scale; j < py * scale + scale; j++) {
                dotMix( x + i, y + j, frontColor);
              }
            }
          }
          else
            for (int i = px * scale; i < px * scale + scale; i++) {
              for (int j = py * scale; j < py * scale + scale; j++) {
                dotMix(x + i, y + j,  backColor);
              }
            }
    }


    //--
    virtual void drawChar90(int x, int y, int ch)
    {
      if (!font)
        return;
      if (!font->valid(ch))
        return;
      const unsigned char *pix = &font->pixels[font->charWidth * font->charHeight * (ch - font->firstChar)];
      for (int py = 0; py < font->charHeight; py++)
        for (int px = font->charWidth; px > 0; px--)
          if (*(pix++))
            dotMix(py + x, px + y, frontColor);
          else
            dotMix(py + x, px + y, backColor);
    }

    void print90(const char ch)
    {
      if (!font)
        return;
      if (font->valid(ch))
        drawChar90(cursorX, cursorY, ch);
      else
        drawChar90(cursorX, cursorY, ' ');
      cursorY -= font->charWidth;
    }

    void print90(const char *str)
    {
      if (!font)
        return;
      while (*str)
      {
        print90(*str);
        str++;
      }
    }

    void printLong90(unsigned long number, int base = 10, int minCharacters = 1)
    {
      if (minCharacters < 1)
        minCharacters = 1;
      const char baseChars[] = "0123456789ABCDEF";
      char temp[33];
      temp[32] = 0;
      int i = 31;
      do
      {
        temp[i--] = baseChars[number % base];
        number /= base;
      } while (number > 0);
      for (; i > 31 - minCharacters; i--)
        temp[i] = ' ';
      print90(&temp[i + 1]);
    }

    void print90(double number, bool left, int fractionalDigits = 2, int minCharacters = 1)
    {
      Serial.println("x");
      long p = long(pow(10, fractionalDigits));
      Serial.print("p: "); Serial.println(p, DEC);
      long long n = (double(number) * p + 0.5f);
      Serial.print("n: "); Serial.println(double(number) * p + 0.5f); Serial.println();
      Serial.println("xx");
      printLong90(long(n / p), 10, minCharacters - 1 - fractionalDigits);
      Serial.println("xxx");
      if (fractionalDigits)
      {
        print90(".");
        Serial.println("xxxx");
        for (int i = 0; i < fractionalDigits; i++)
        {
          p /= 10;
          printLong90(long(n / p) % 10);
          Serial.println("xxxxx");
        }
      }
    }
    //--

    void printScale(const char ch, int scale)
    {
      if (!font)
        return;
      if (font->valid(ch))
        drawCharScale(cursorX, cursorY, scale, ch);
      else
        drawCharScale(cursorX, cursorY, scale, ' ');
      cursorX += font->charWidth * scale;
      if (cursorX + (font->charWidth)*scale > xres)
      {
        cursorX = cursorBaseX;
        cursorY += (font->charHeight) * scale;
        if (autoScroll && cursorY + (font->charHeight)*scale > yres)
          scroll(cursorY + (font->charHeight)*scale - yres, backColor); //  <---- vermutlich anpassen
      }
    }

    void printlnScale(const char ch, int scale)
    {
      printScale(ch, scale);
      printScale("\n", scale);
    }

    void printScale(const char *str, int scale)
    {
      if (!font)
        return;
      while (*str)
      {
        if (*str == '\n')
        {
          cursorX = cursorBaseX;
          cursorY += font->charHeight * scale;
        }
        else
          printScale(*str, scale);
        str++;
      }
    }

    void printScaleM(const char *str, int scale, int marker)
    {
      if (!font)
        return;

      int counter = 0;
      while (*str)
      {
        counter++;
        if (counter == marker) {
          setTextColor(RGB(255, 0, 0));
        } else {
          setTextColor(RGB(0, 0, 0));
        }
        if (*str == '\n')
        {
          cursorX = cursorBaseX;
          cursorY += font->charHeight * scale;
        }
        else
          printScale(*str, scale);
        str++;
      }
    }

    void printSC(const char *str, int scale)
    {
      if (!font)
        return;
      int len = strlen(str);

      int cursorXT = cursorX;
      int cursorYT = cursorY;
      int stringXScale = font->charWidth * scale * len;
      int stringYScale = font->charHeight * scale;
      setCursor((xres / 2) - (stringXScale / 2), (yres / 2) - (stringYScale / 2));
      printScale(str, scale);
      setCursor(cursorXT, cursorYT);
    }


    void printSCM(const char *str, int scale, int marker) {
      printSCM(str, scale, marker, false);
    }

    void printSCM(const char *str, int scale, int marker, boolean box)
    {
      if (!font)
        return;
      int len = strlen(str);

      if (marker > len) {
        return;
      }

      int cursorXT = cursorX;
      int cursorYT = cursorY;
      int stringXScale = font->charWidth * scale * len;
      int stringYScale = font->charHeight * scale;
      if (box) {
        fillRect(((xres / 2) - (stringXScale / 2))-10, ((yres / 2) - (stringYScale / 2))-10, (stringXScale-scale)+20, (stringYScale-scale)+20, RGB(255,255,255));
        rect(((xres / 2) - (stringXScale / 2))-10, ((yres / 2) - (stringYScale / 2))-10, (stringXScale-scale)+20, (stringYScale-scale)+20, RGB(0,0,0));
      }
      setCursor((xres / 2) - (stringXScale / 2), (yres / 2) - (stringYScale / 2));
      printScaleM(str, scale, marker);
      setCursor(cursorXT, cursorYT);
    }

    void printlnScale(const char *str, int scale)
    {
      printScale(str, scale);
      printScale("\n", scale);
    }

    void printScale(long number, int scale, int base = 10, int minCharacters = 1)
    {
      if (minCharacters < 1)
        minCharacters = 1;
      bool sign = number < 0;
      if (sign)
        number = -number;
      const char baseChars[] = "0123456789ABCDEF";
      char temp[33];
      temp[32] = 0;
      int i = 31;
      do
      {
        temp[i--] = baseChars[number % base];
        number /= base;
      } while (number > 0);
      if (sign)
        temp[i--] = '-';
      for (; i > 31 - minCharacters; i--)
        temp[i] = ' ';
      printScale(&temp[i + 1], scale);
    }

    void printScale(unsigned long number, int scale, int base = 10, int minCharacters = 1)
    {
      if (minCharacters < 1)
        minCharacters = 1;
      const char baseChars[] = "0123456789ABCDEF";
      char temp[33];
      temp[32] = 0;
      int i = 31;
      do
      {
        temp[i--] = baseChars[number % base];
        number /= base;
      } while (number > 0);
      for (; i > 31 - minCharacters; i--)
        temp[i] = ' ';
      printScale(&temp[i + 1], scale);
    }

    void printlnScale(long number, int scale, int base = 10, int minCharacters = 1)
    {
      printScale(number, scale, base, minCharacters); printScale("\n", scale);
    }

    void printlnScale(unsigned long number, int scale, int base = 10, int minCharacters = 1)
    {
      printScale(number, scale, base, minCharacters); printScale("\n", scale);
    }

    void printScale(int number, int scale, int base = 10, int minCharacters = 1)
    {
      printScale(long(number), scale, base, minCharacters);
    }

    void printlnScale(int number, int scale, int base = 10, int minCharacters = 1)
    {
      printlnScale(long(number), scale, base, minCharacters);
    }

    void printScale(unsigned int number, int scale, int base = 10, int minCharacters = 1)
    {
      printScale((unsigned long)(number), scale, base, minCharacters);
    }

    void printlnScale(unsigned int number, int scale, int base = 10, int minCharacters = 1)
    {
      printlnScale((unsigned long)(number), scale, base, minCharacters);
    }

    void printScale(short number, int scale, int base = 10, int minCharacters = 1)
    {
      printScale(long(number), scale, base, minCharacters);
    }

    void printlnScale(short number, int scale, int base = 10, int minCharacters = 1)
    {
      printlnScale(long(number), scale, base, minCharacters);
    }

    void printScale(unsigned short number, int scale, int base = 10, int minCharacters = 1)
    {
      printScale(long(number), scale, base, minCharacters);
    }

    void printlnScale(unsigned short number, int scale, int base = 10, int minCharacters = 1)
    {
      printlnScale(long(number), scale, base, minCharacters);
    }

    void printScale(unsigned char number, int scale, int base = 10, int minCharacters = 1)
    {
      printScale(long(number), scale, base, minCharacters);
    }

    void printlnScale(unsigned char number, int scale, int base = 10, int minCharacters = 1)
    {
      printlnScale(long(number), scale, base, minCharacters);
    }

    void printlnScale(int scale)
    {
      printScale("\n", scale);
    }

    void printScale(double number, int scale, int fractionalDigits = 2, int minCharacters = 1)
    {
      long p = long(pow(10, fractionalDigits));
      long long n = (double(number) * p + 0.5f);
      printScale(long(n / p), scale, 10, minCharacters - 1 - fractionalDigits);
      if (fractionalDigits)
      {
        printScale(".", scale);
        for (int i = 0; i < fractionalDigits; i++)
        {
          p /= 10;
          printScale(long(n / p) % 10, scale);
        }
      }
    }

    void printlnScale(double number, int scale, int fractionalDigits = 2, int minCharacters = 1)
    {
      printScale(number, scale, fractionalDigits, minCharacters);
      printScale("\n", scale);
    }

    //--
    int getCursorX() {
      return cursorX;
    }
    int getCursorY() {
      return cursorY;
    }

void startVGA(){
      I2S::startTX();
      }
    
    void stopVGA(){
      I2S::i2sStop();
      }
};
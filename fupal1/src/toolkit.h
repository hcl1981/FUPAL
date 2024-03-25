/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "SPIFFS.h"
//#include "soc/rtc_wdt.h"
//#include "esp_task_wdt.h"

#define SW1 36
#define SW2 39
#define SW3 34
#define SW4 35
#define SW5 14

#define BLACK vga.RGB(0, 0, 0)
#define WHITE vga.RGB(255, 255, 255)
#define RED vga.RGB(255, 0, 0)
#define GREEN vga.RGB(0, 255, 0)
#define BLUE vga.RGB(0, 0, 255)
#define CYAN vga.RGB(0, 255, 255)
#define MAGENTA vga.RGB(255, 0, 255)
#define YELLOW vga.RGB(255, 255, 0)

using namespace std;

TaskHandle_t Task1;
TaskHandle_t Task2;

void waitForButton()
{
  while (digitalRead(SW1) == LOW || digitalRead(SW2) == LOW || digitalRead(SW3) == LOW || digitalRead(SW4) == LOW || digitalRead(SW5) == LOW)
  {
  }
  delay(100);
  while (digitalRead(SW1) == HIGH && digitalRead(SW2) == HIGH && digitalRead(SW3) == HIGH && digitalRead(SW4) == HIGH && digitalRead(SW5) == HIGH)
  {
  }
}

/*
class BMP
{
private:
  VGA3BitIscale vga;

public:
  BMP()
  {
  }
  BMP(VGA3BitIscale pVGA)
  {
    vga = pVGA;
  }

  void displayBMP(const char *path)
  {
    bool ok = true;
    vga.clear(vga.RGB(255, 255, 255));
    vga.stopVGA();
    if (!SPIFFS.begin(false))
    {
      Serial.println("SPIFFS Mount Failed");
      ok = false;
    }

    File file = SPIFFS.open(path);
    if (ok && (!file || file.isDirectory()))
    {
      Serial.println("failed to open file for reading");
      ok = false;
    }

    if (ok)
    {
      for (int i = 0; i < 130; i++)
      {
        file.read();
      }
      int inp;
      delay(5);
      for (int row = 480; row > 0; row--)
      {
        for (int col = 0; col < 80; col++)
        {
          inp = file.read();
          //Serial.println(inp);
          if ((inp & 1) > 0)
          {
            vga.dot((col * 8) + 7, row, BLACK);
          }
          if ((inp & 2) > 0)
          {
            vga.dot((col * 8) + 6, row, BLACK);
          }
          if ((inp & 4) > 0)
          {
            vga.dot((col * 8) + 5, row, BLACK);
          }
          if ((inp & 8) > 0)
          {
            vga.dot((col * 8) + 4, row, BLACK);
          }
          if ((inp & 16) > 0)
          {
            vga.dot((col * 8) + 3, row, BLACK);
          }
          if ((inp & 32) > 0)
          {
            vga.dot((col * 8) + 2, row, BLACK);
          }
          if ((inp & 64) > 0)
          {
            vga.dot((col * 8) + 1, row, BLACK);
          }
          if ((inp & 128) > 0)
          {
            vga.dot((col * 8), row, BLACK);
          }
        }
      }
      vga.startVGA();
    }
    else
    {
      vga.setTextColor(BLACK);
      vga.printSC(IMG_ERR, 2);
      vga.startVGA();
      delay(2000);
    }
    //Serial.println("Starte VGA!");

    //delay(2000);
    //Serial.println("Fertig!");
  }
};
*/

class BMP
{
private:
  VGA3BitIscale vga;

public:
  BMP()
  {
  }
  BMP(VGA3BitIscale pVGA)
  {
    vga = pVGA;
  }

  void displayBMP(const char *path)
  {
    bool ok = true;
    vga.clear(vga.RGB(0, 0, 0));
    vga.stopVGA();
    if (!SPIFFS.begin(false))
    {
      vga.clear(vga.RGB(255,255,255));
      Serial.println("SPIFFS Mount Failed");
      ok = false;
    }

    File file = SPIFFS.open(path);
    if (ok && (!file || file.isDirectory()))
    {
      vga.clear(vga.RGB(255,255,255));
      Serial.println("failed to open file for reading");
      ok = false;
    }

    if (ok)
    {
      for (int i = 0; i < 62; i++)
      {
        file.read();
      }
      int inp;
      delay(5);
      for (int row = 480; row > 0; row--)
      {
        for (int col = 0; col < 80; col++)
        {
          inp = file.read();
          //Serial.println(inp);
          if ((inp & 1) > 0)
          {
            vga.dot((col * 8) + 7, row, WHITE);
          }
          if ((inp & 2) > 0)
          {
            vga.dot((col * 8) + 6, row, WHITE);
          }
          if ((inp & 4) > 0)
          {
            vga.dot((col * 8) + 5, row, WHITE);
          }
          if ((inp & 8) > 0)
          {
            vga.dot((col * 8) + 4, row, WHITE);
          }
          if ((inp & 16) > 0)
          {
            vga.dot((col * 8) + 3, row, WHITE);
          }
          if ((inp & 32) > 0)
          {
            vga.dot((col * 8) + 2, row, WHITE);
          }
          if ((inp & 64) > 0)
          {
            vga.dot((col * 8) + 1, row, WHITE);
          }
          if ((inp & 128) > 0)
          {
            vga.dot((col * 8), row, WHITE);
          }
        }
      }
      vga.startVGA();
    }
    else
    {
      vga.clear(vga.RGB(255,255,255));
      vga.setTextColor(BLACK);
      vga.printSC(IMG_ERR, 2);
      vga.startVGA();
      delay(2000);
    }
    //Serial.println("Starte VGA!");

    //delay(2000);
    //Serial.println("Fertig!");
  }
};

void showImage(char *file, VGA3BitIscale vga)
{
  BMP bmp(vga);
  bmp.displayBMP(file);
}

class UI
{
private:
  VGA3BitIscale vga;

public:
  UI()
  {
  }
  UI(VGA3BitIscale pVGA)
  {
    vga = pVGA;
  }

  void drawLegend(const char *m1, const char *m2, const char *m3, const char *m4, const char *m5)
  {
    vga.fillRect(0, 455, vga.xres, 480, WHITE);
    vga.line(0, 455, vga.xres, 455, BLACK);
    vga.line(1 * vga.xres / 5, 455, 1 * vga.xres / 5, vga.yres, BLACK);
    vga.line(2 * vga.xres / 5, 455, 2 * vga.xres / 5, vga.yres, BLACK);
    vga.line(3 * vga.xres / 5, 455, 3 * vga.xres / 5, vga.yres, BLACK);
    vga.line(4 * vga.xres / 5, 455, 4 * vga.xres / 5, vga.yres, BLACK);
    vga.setCursor(64 - (vga.font->charWidth * strlen(m1)) / 2, 460);
    vga.print(m1);
    vga.setCursor(192 - (vga.font->charWidth * strlen(m2)) / 2, 460);
    vga.print(m2);
    vga.setCursor(320 - (vga.font->charWidth * strlen(m3)) / 2, 460);
    vga.print(m3);
    vga.setCursor(448 - (vga.font->charWidth * strlen(m4)) / 2, 460);
    vga.print(m4);
    vga.setCursor(576 - (vga.font->charWidth * strlen(m5)) / 2, 460);
    vga.print(m5);
  }

  int drawSelection(const char *m_menu[20], int entries, boolean info)
  {
    if (info)
    {
      drawLegend("\x1F", "\x1E", "", "Info", "Auswahl");
    }
    else
    {
      drawLegend("\x1F", "\x1E", "", "", "Auswahl");
    }
    int selection = 0;
    vga.rect(45, 35, 550, 390, BLACK);
    for (int i = 0; i < entries; i++)
    {
      vga.setCursor(70, 53 + i * (vga.font->charHeight + 2));
      if (m_menu[i] != NULL)
      {
        vga.print(m_menu[i]);
      }
    }
    vga.setCursor(55, 53 + selection * (vga.font->charHeight + 2));
    vga.print(">");
    while (digitalRead(SW5) == LOW)
    {
    };
    delay(100);
    while (true)
    {
      if (digitalRead(SW1) == LOW)
      {
        vga.setCursor(55, 53 + selection * (vga.font->charHeight + 2));
        vga.setTextColor(WHITE);
        vga.print(">");
        vga.setTextColor(BLACK);
        selection++;
        if (selection == entries)
        {
          selection = 0;
        }
        vga.setCursor(55, 53 + selection * (vga.font->charHeight + 2));
        vga.print(">");
        delay(200);
      }

      if (digitalRead(SW2) == LOW)
      {
        vga.setCursor(55, 53 + selection * (vga.font->charHeight + 2));
        vga.setTextColor(WHITE);
        vga.print(">");
        vga.setTextColor(BLACK);
        selection--;
        if (selection == -1)
        {
          selection = entries - 1;
        }
        vga.setCursor(55, 53 + selection * (vga.font->charHeight + 2));
        vga.print(">");
        delay(200);
      }
      if (digitalRead(SW5) == LOW)
      {
        vga.clear(WHITE);
        return selection;
      }
      if (info && (digitalRead(SW4) == LOW))
      {
        vga.clear(WHITE);
        return selection + 100;
      }
    }
  }

  char *numberInput(char *input)
  {
    int pos = 1;
    int len = 0;
    char *output = strdup(input);
    while (*input)
    {
      input++;
      len++;
    }
    vga.fillRect(0, 0, 640, 454, WHITE);
    vga.printSCM(output, 3, pos);
    drawLegend("+", "-", "\x11", "\x10", B_SELECT);
    while (digitalRead(SW5) == HIGH)
    {
      if (digitalRead(SW1) == LOW)
      {
        char curr = output[pos - 1];
        curr++;
        if (curr > '9')
        {
          curr = '0';
        }
        output[pos - 1] = curr;
        vga.fillRect(0, 0, 640, 454, WHITE);
        vga.printSCM(output, 3, pos);
        delay(200);
      }
      if (digitalRead(SW2) == LOW)
      {
        char curr = output[pos - 1];
        curr--;
        if (curr < '0')
        {
          curr = '9';
        }
        output[pos - 1] = curr;
        vga.fillRect(0, 0, 640, 454, WHITE);
        vga.printSCM(output, 3, pos);
        delay(200);
      }
      if (digitalRead(SW3) == LOW)
      {
        pos--;
        if (pos <= 0)
        {
          pos = len;
        }

        while (output[pos - 1] > '9' || output[pos - 1] < '0')
        {
          pos--;
          if (pos <= 0)
          {
            pos = len;
          }
        }

        vga.fillRect(0, 0, 640, 454, WHITE);
        vga.printSCM(output, 3, pos);
        delay(200);
      }
      if (digitalRead(SW4) == LOW)
      {
        pos++;
        if (pos >= len + 1)
        {
          pos = 1;
        }

        while (output[pos - 1] > '9' || output[pos - 1] < '0')
        {
          pos++;
          if (pos >= len + 1)
          {
            pos = 1;
          }
        }

        vga.fillRect(0, 0, 640, 454, WHITE);
        vga.printSCM(output, 3, pos);
        delay(200);
      }
    }
    vga.clear(WHITE);
    vga.setTextColor(BLACK);
    return output;
  }
};

class Table
{
public:
  Table(VGA3BitIscale &tvga, UI t_ui, int t_columns, int t_rows, boolean t_half)
  {
    half = t_half;
    vga = tvga;
    ui = t_ui;
    rows = t_rows;
    columns = t_columns;
    //create table
    table = new float *[columns];
    for (int i = 0; i < columns; i++)
    {
      table[i] = new float[rows];
    }
    // fill with -1
    for (int i = 0; i < columns; i++)
    {
      for (int j = 0; j < rows; j++)
      {
        table[i][j] = -1;
      }
    }
  }

  void clean()
  {
    for (int i = 0; i < columns; i++)
    {
      for (int j = 0; j < rows; j++)
      {
        table[i][j] = -1;
      }
    }
  }

  void setColumns(char *pColumns[10])
  {
    cols = pColumns;
  }

  void setInts(boolean pInts[10])
  {
    ints = pInts;
  }

  void setValue(int column, int row, float val)
  {
    table[column][row] = val;
  }

  void setValueDraw(int column, int row, float val)
  {
    int width = vga.xres;
    if (half)
    {
      width /= 2;
    }
    table[column][row] = val;
    vga.fillRect(((width / columns) * column) + 1, 24 + (vga.font->charHeight) * row, (width / columns) - 2, vga.font->charHeight, WHITE);
    vga.setCursor((width / columns) * column + 5, 24 + (vga.font->charHeight) * row);
    if (strcmp(cols[column], NUM) == 0)
    {
      vga.print((int)val);
    }
    else
    {
      vga.print(val);
    }
  }

  float getValue(int column, int row)
  {
    return table[column][row];
  }

  float **getData()
  {
    return table;
  }

  void draw()
  {
    int width = vga.xres;
    if (half)
    {
      width /= 2;
    }

    vga.fillRect(0, 0, width, 440, WHITE);
    vga.line(0, 20, width, 20, BLACK);
    for (int i = 1; i < columns; i++)
    {
      vga.line((width / columns) * i, 0, (width / columns) * i, 440, BLACK);
      vga.setCursor((width / columns) * i + 5, 0);
    }

    for (int i = 0; i < columns; i++)
    {
      vga.setCursor((width / columns) * i + 5, 4);
      vga.print(cols[i]);
    }

    for (int i = 0; i < columns; i++)
    {
      for (int j = 0; j < rows; j++)
      {
        vga.setCursor((width / columns) * i + 5, 24 + (vga.font->charHeight) * j);

        if (i == selected_x && j == selected_y)
        {
          vga.setTextColor(RED);
        }

        if (table[i][j] == -1)
        {

          vga.print("---");
        }
        else
        {
          //if (cols[i] == "Nr.")
          if (strcmp(cols[i], NUM) == 0)
          {
            vga.print((int)table[i][j]);
          }
          else
          {
            vga.print(table[i][j]);
          }
        }

        if (i == selected_x && j == selected_y)
        {
          vga.setTextColor(BLACK);
        }
      }
    }
  }

  void setSelection(int x, int y)
  {
    selected_x = x;
    selected_y = y;
  }

  int getSelectedColumn()
  {
    return selected_x;
  }

  int getSelectedRow()
  {
    return selected_y;
  }

  float getSelectedValue()
  {
    return table[selected_x][selected_y];
  }

  void setSelectedValue(float val)
  {
    table[selected_x][selected_y] = val;
  }

  void markCurrent(bool mark)
  {
    int width = vga.xres;
    if (half)
    {
      width /= 2;
    }
    vga.setCursor((width / columns) * selected_x + 5, 24 + (vga.font->charHeight) * selected_y);

    if (mark)
    {
      vga.setTextColor(RED);
    }
    else
    {
      vga.setTextColor(BLACK);
    }
    if (table[selected_x][selected_y] == -1)
    {
      vga.print("---");
    }
    else
    {
      vga.print(table[selected_x][selected_y]);
    }
    vga.setTextColor(BLACK);
  }
  void up()
  {
    markCurrent(false);

    selected_y--;
    if (selected_y < 0)
    {
      selected_y = rows - 1;
    }
    markCurrent(true);
  }
  void down()
  {
    markCurrent(false);
    selected_y++;
    if (selected_y >= rows)
    {
      selected_y = 0;
    }
    markCurrent(true);
  }
  void left()
  {
    markCurrent(false);
    selected_x--;
    if (selected_x < 0)
    {
      selected_x = columns - 1;
    }
    markCurrent(true);
  }
  void right()
  {
    markCurrent(false);
    selected_x++;
    if (selected_x >= columns)
    {
      selected_x = 0;
    }
    markCurrent(true);
  }

  int getRows()
  {
    return rows;
  }

  int getColumns()
  {
    return columns;
  }

private:
  VGA3BitIscale vga;
  float **table;
  int rows, columns;
  char **cols;
  boolean *ints;
  bool half;
  int selected_x = -1;
  int selected_y = -1;
  UI ui;
};

class Graph
{
public:
  Graph(VGA3BitIscale &tvga, int t_columns, int t_rows, boolean t_half)
  {
    half = t_half;
    vga = tvga;
    rows = t_rows;
    columns = t_columns;
    //create table
    table = new float *[columns];
    for (int i = 0; i < columns; i++)
    {
      table[i] = new float[rows];
    }
    // fill with -1
    for (int i = 0; i < columns; i++)
    {
      for (int j = 0; j < rows; j++)
      {
        table[i][j] = -1;
      }
    }
  }

  float limit(float input, bool upper)
  {
    if (input == -1)
      return -1;
    float rkg = 0;
    int exp = 0;
    for (int i = -6; i < 7; i++)
    {
      if (input > pow(10, i))
      {
        exp = i;
      };
    }

    exp--;

    do
    {
      rkg += pow(10, exp);
    } while (rkg < input);

    if (upper)
    {
      rkg += pow(10, exp);
    }
    else
    {
      rkg -= pow(10, exp);
    }

    if (rkg < 0)
    {
      return 0;
    }
    else
    {
      return rkg;
    }
  }
  int mapf(double val, double in_min, double in_max, double out_min, double out_max)
  {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  void draw(float **table)
  {
    drawColNull(table, 1, false);
  }

  void drawColNull(float **table, int col, boolean startAtNull)
  {
    //startatnull implementieren
    int left;
    if (half)
    {
      left = 320;
      vga.fillRect(320, 0, 320, 440, WHITE);
    }
    else
    {
      left = 0;
      vga.fillRect(0, 0, 640, 440, WHITE);
    }

    float min_x = -1;
    float max_x = -1;
    float min_y = -1;
    float max_y = -1;

    for (int i = 0; i < rows; i++)
    {

      if (table[0][i] != -1)
      {
        if (table[0][i] < min_x || min_x == -1)
        {
          min_x = table[0][i];
        }
        if (table[0][i] > max_x || min_x == -1)
        {
          max_x = table[0][i];
        }
      }

      if (table[col][i] != -1)
      {
        if (table[col][i] < min_y || min_y == -1)
        {
          min_y = table[col][i];
        }
        if (table[col][i] > max_y || min_y == -1)
        {
          max_y = table[col][i];
        }
      }
    }

    min_x = limit(min_x, false);
    min_y = limit(min_y, false);
    max_x = limit(max_x, true);
    max_y = limit(max_y, true);

    if (min_x != max_x && min_y != max_y)
    {

      vga.line(left + vga.font->charHeight, 10, left + vga.font->charHeight, 440, BLACK);
      vga.line(left + vga.font->charHeight, 440, 640 - 10, 440, BLACK);

      char buf[12];
      vga.setCursor(left, 440 - vga.font->charWidth);
      if (startAtNull)
      {
        sprintf(buf, "%.2f", 0.0);
      }
      else
      {
        sprintf(buf, "%.2f", min_y);
      }
      vga.print90(buf);
      vga.setCursor(left, 10 + (strlen(buf) - 1) * vga.font->charWidth);
      sprintf(buf, "%.2f", max_y);
      vga.print90(buf);
      vga.setCursor(left + vga.font->charHeight, 440);
      if (startAtNull)
      {
        sprintf(buf, "%.2f", 0.0);
      }
      else
      {
        sprintf(buf, "%.2f", min_x);
      }
      vga.print(buf);
      vga.setCursor(640 - 10 - (strlen(buf) + 1) * vga.font->charWidth, 440);
      sprintf(buf, "%.2f", max_x);
      vga.print(buf);

      int tx;
      int ty;

      for (int i = 0; i < rows; i++)
      {
        if (table[0][i] != -1 && table[col][i] != -1)
        {
          if (startAtNull)
          {
            tx = mapf(table[0][i], 0, max_x, left + vga.font->charHeight, 640 - 10);
          }
          else
          {
            tx = mapf(table[0][i], min_x, max_x, left + vga.font->charHeight, 640 - 10);
          }
          if (startAtNull)
          {
            ty = 450 - mapf(table[col][i], 0, max_y, 10, 440);
          }
          else
          {
            ty = 450 - mapf(table[col][i], min_y, max_y, 10, 440);
          }
          vga.dot(tx, ty, RED);
          vga.dot(tx - 1, ty - 1, RED);
          vga.dot(tx - 1, ty + 1, RED);
          vga.dot(tx + 1, ty - 1, RED);
          vga.dot(tx + 1, ty + 1, RED);
          vga.dot(tx - 2, ty - 2, RED);
          vga.dot(tx - 2, ty + 2, RED);
          vga.dot(tx + 2, ty - 2, RED);
          vga.dot(tx + 2, ty + 2, RED);
        }
      }
    }
  }

private:
  VGA3BitIscale vga;
  float **table;
  int rows, columns;
  bool half;
};

//Vgl. http://www.getelectronics.net/wemos-bh1750fvi-example.php
class LUX
{
public:
  byte buff[2];
  int BH1750_Read(int address) //
  {
    int i = 0;
    Wire.beginTransmission(address);
    Wire.requestFrom(address, 2);
    while (Wire.available()) //
    {
      buff[i] = Wire.read(); // receive one byte
      i++;
    }
    Wire.endTransmission();
    return i;
  }

  void BH1750_Init(int address)
  {
    Wire.beginTransmission(address);
    Wire.write(0x10); //1lx reolution 120ms
    Wire.endTransmission();
  }
};

class SDRW
{
public:
  char *newFilename()
  {
    static char fname[12];
    for (int i = 1; i < 100000; i++)
    {
      sprintf(fname, "/%05d.csv", i);
      File file = SD.open(fname);
      if (!file)
      {
        return fname;
      }
    }
    return NULL;
  }

  bool writeFile(const char *path, const char *message)
  {
    File file = SD.open(path, FILE_WRITE);
    if (!file)
    {
      return false;
    }
    if (file.print(message))
    {
      return true;
    }
    else
    {
      return false;
    }
    file.close();
  }

  bool appendFile(const char *path, const char *message)
  {
    File file = SD.open(path, FILE_APPEND);
    if (!file)
    {
      return false;
    }
    if (file.print(message))
    {
      return true;
    }
    else
    {
      return false;
    }
    file.close();
  }

  void mount()
  {
    if (!SD.begin(13))
    {
      Serial.println("Card Mount Failed");
      return;
    }
  }
};

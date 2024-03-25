/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

#include "font.h"
#include <JPEGDecoder.h>

#define GFXFF 1
#define LINEFEED 22

#define SW1 36
#define SW2 39
#define SW3 34
#define SW4 35
#define SW5 14

#define SANSBOLD9 &LiberationSans_Bold9pt8b

TaskHandle_t Task1;
TaskHandle_t Task2;

void drawString90(const char *string, int xw, int yh, TFT_eSPI tft)
{
  TFT_eSprite spr = TFT_eSprite(&tft);
  tft.setPivot(xw, yh);
  spr.setFreeFont(SANSBOLD9);
  spr.setColorDepth(8);
  spr.createSprite(tft.textWidth(string), 20);
  spr.setPivot(0, 0);
  spr.fillSprite(TFT_BLACK);
  spr.setTextColor(TFT_GREEN);
  spr.setTextDatum(TL_DATUM);
  spr.drawString(string, 0, 0);
  spr.pushRotated(270);
}

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

void printMessage(const char *message, TFT_eSPI tft)
{
  tft.setTextDatum(MC_DATUM);
  tft.fillRect(tft.width() / 2 - (tft.textWidth(message) / 2 + 23), tft.height() / 2 - 43, tft.textWidth(message) + 46, 46, TFT_BLACK);
  tft.drawRect(tft.width() / 2 - (tft.textWidth(message) / 2 + 20), tft.height() / 2 - 40, tft.textWidth(message) + 40, 40, TFT_GREEN);
  tft.drawRect(tft.width() / 2 - (tft.textWidth(message) / 2 + 23), tft.height() / 2 - 43, tft.textWidth(message) + 46, 46, TFT_GREEN);
  tft.drawString(message, tft.width() / 2, tft.height() / 2 - 23, GFXFF);
}

void clearFrame(const char *message, TFT_eSPI tft)
{
  tft.setTextDatum(MC_DATUM);
  tft.drawRect(tft.width() / 2 - (tft.textWidth(message) / 2 + 20), tft.height() / 2 - 40, tft.textWidth(message) + 40, 40, TFT_BLACK);
  tft.drawRect(tft.width() / 2 - (tft.textWidth(message) / 2 + 23), tft.height() / 2 - 43, tft.textWidth(message) + 46, 46, TFT_BLACK);
}

void drawLegend(const char *m1, const char *m2, const char *m3, const char *m4, const char *m5, TFT_eSPI tft)
{
  tft.setTextDatum(BC_DATUM);
  tft.fillRect(0, tft.height() - 25, tft.width(), 25, TFT_BLACK);
  tft.drawLine(0, tft.height() - 25, tft.width(), tft.height() - 25, TFT_GREEN);
  tft.drawLine(1 * tft.width() / 5, tft.height() - 25, 1 * tft.width() / 5, tft.height(), TFT_GREEN);
  tft.drawLine(2 * tft.width() / 5, tft.height() - 25, 2 * tft.width() / 5, tft.height(), TFT_GREEN);
  tft.drawLine(3 * tft.width() / 5, tft.height() - 25, 3 * tft.width() / 5, tft.height(), TFT_GREEN);
  tft.drawLine(4 * tft.width() / 5, tft.height() - 25, 4 * tft.width() / 5, tft.height(), TFT_GREEN);
  tft.drawString(m1, 0.5 * tft.width() / 5, tft.height() - 3, GFXFF);
  tft.drawString(m2, 1.5 * tft.width() / 5, tft.height() - 3, GFXFF);
  tft.drawString(m3, 2.5 * tft.width() / 5, tft.height() - 3, GFXFF);
  tft.drawString(m4, 3.5 * tft.width() / 5, tft.height() - 3, GFXFF);
  tft.drawString(m5, 4.5 * tft.width() / 5, tft.height() - 3, GFXFF);
}

int drawSelection(const char *m_menu[20], int entries, bool back, bool info, TFT_eSPI tft)
{
  tft.setTextDatum(ML_DATUM);

  if (info)
  {
    if (back)
    {
      drawLegend("§", "$", "Zurück", "Info", "Auswahl", tft);
    }
    else
    {
      drawLegend("§", "$", "", "Info", "Auswahl", tft);
    }
  }
  else
  {
    if (back)
    {
      drawLegend("§", "$", "Zurück", "", "Auswahl", tft);
    }
    else
    {
      drawLegend("§", "$", "", "", "Auswahl", tft);
    }
  }

  int selection = 0;
  tft.drawRect(25, 25, tft.width() - 50, tft.height() - 75, TFT_GREEN);
  for (int i = 0; i < entries; i++)
  {
    if (m_menu[i] != NULL)
    {
      tft.drawString(m_menu[i], 65, 55 + (LINEFEED * i), GFXFF);
    }
  }
  tft.drawString(">", 40, 55 + (LINEFEED * selection), GFXFF);

  while (digitalRead(SW5) == LOW)
  {
  };
  delay(100);
  while (true)
  {
    if (digitalRead(SW1) == LOW)
    {
      tft.setTextColor(TFT_BLACK);
      tft.drawString(">", 40, 55 + (LINEFEED * selection), GFXFF);
      tft.setTextColor(TFT_GREEN);
      selection++;
      if (selection == entries)
      {
        selection = 0;
      }

      tft.drawString(">", 40, 55 + (LINEFEED * selection), GFXFF);
      delay(200);
    }

    if (digitalRead(SW2) == LOW)
    {
      tft.setTextColor(TFT_BLACK);
      tft.drawString(">", 40, 55 + (LINEFEED * selection), GFXFF);
      tft.setTextColor(TFT_GREEN);
      selection--;
      if (selection == -1)
      {
        selection = entries - 1;
      }

      tft.drawString(">", 40, 55 + (LINEFEED * selection), GFXFF);
      delay(200);
    }
    if (back && (digitalRead(SW3) == LOW))
    {
      tft.fillScreen(TFT_BLACK);
      return -1;
    }

    if ((digitalRead(SW4) == LOW) && info)
    {
      tft.fillScreen(TFT_BLACK);
      return selection + 100;
    }

    if (digitalRead(SW5) == LOW)
    {
      tft.fillScreen(TFT_BLACK);
      return selection;
    }
  }
}

int drawSelectionInfoscreen(const char *linetext[20], int lines, const char *m_menu[20], int entries, bool back, TFT_eSPI tft)
{
  tft.setTextDatum(ML_DATUM);

  if (back)
  {
    drawLegend("§", "$", "Zurück", "", "Auswahl", tft);
  }
  else
  {
    drawLegend("§", "$", "", "", "Auswahl", tft);
  }

  for (int i = 0; i < lines; i++)
  {
    tft.drawString(linetext[i], 65, 55 + LINEFEED * i, GFXFF);
  }

  int selection = 0;
  tft.drawRect(25, 25, tft.width() - 50, tft.height() - 75, TFT_GREEN);
  for (int i = 0; i < entries; i++)
  {
    if (m_menu[i] != NULL)
    {
      tft.drawString(m_menu[i], 65, (55 + LINEFEED * (lines + 1)) + (LINEFEED * i), GFXFF);
    }
  }
  tft.drawString(">", 40, (55 + LINEFEED * (lines + 1)) + (LINEFEED * selection), GFXFF);

  while (digitalRead(SW5) == LOW)
  {
  };
  delay(100);
  while (true)
  {
    if (digitalRead(SW1) == LOW)
    {
      tft.setTextColor(TFT_BLACK);
      tft.drawString(">", 40, (55 + LINEFEED * (lines + 1)) + (LINEFEED * selection), GFXFF);
      tft.setTextColor(TFT_GREEN);
      selection++;
      if (selection == entries)
      {
        selection = 0;
      }

      tft.drawString(">", 40, (55 + LINEFEED * (lines + 1)) + (LINEFEED * selection), GFXFF);
      delay(200);
    }

    if (digitalRead(SW2) == LOW)
    {
      tft.setTextColor(TFT_BLACK);
      tft.drawString(">", 40, (55 + LINEFEED * (lines + 1)) + (LINEFEED * selection), GFXFF);
      tft.setTextColor(TFT_GREEN);
      selection--;
      if (selection == -1)
      {
        selection = entries - 1;
      }

      tft.drawString(">", 40, (55 + LINEFEED * (lines + 1)) + (LINEFEED * selection), GFXFF);
      delay(200);
    }
    if (back && (digitalRead(SW3) == LOW))
    {
      tft.fillScreen(TFT_BLACK);
      return -1;
    }

    if (digitalRead(SW5) == LOW)
    {
      tft.fillScreen(TFT_BLACK);
      return selection;
    }
  }
}

class Table
{
public:
  Table(int t_columns, int t_rows, boolean t_half)
  {
    half = t_half;
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

  void setValueDraw(int column, int row, float val, TFT_eSPI tft)
  {

    tft.setTextDatum(TC_DATUM);
    int width = tft.width();
    if (half)
    {
      width /= 2;
    }
    table[column][row] = val;
    tft.fillRect(((width / columns) * column) + 1, 25 + row * LINEFEED, (width / columns) - 2, LINEFEED, TFT_BLACK);
    if (strcmp(cols[column], "Nr.") == 0)
    {
      tft.drawString(String((int)val), (width / columns) * (column + 0.5), 25 + row * LINEFEED, GFXFF);
    }
    else
    {
      tft.drawString(String(val), (width / columns) * (column + 0.5), 25 + row * LINEFEED, GFXFF);
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

  void draw(TFT_eSPI tft)
  {
    tft.setTextDatum(TC_DATUM);
    int width = tft.width();
    if (half)
    {
      width /= 2;
    }

    tft.fillRect(0, 0, width, tft.height() - 50, TFT_BLACK);
    tft.drawLine(0, 21, width, 21, TFT_GREEN);
    for (int i = 1; i < columns; i++)
    {
      tft.drawLine((width / columns) * i, 0, (width / columns) * i, tft.height() - 50, TFT_GREEN);
    }
    for (int i = 0; i < columns; i++)
    {
      tft.drawString(cols[i], (width / columns) * (i + 0.5), 1, GFXFF);
    }
    for (int i = 0; i < columns; i++)
    {
      for (int j = 0; j < rows; j++)
      {
        if (i == selected_x && j == selected_y)
        {
          tft.setTextColor(TFT_RED);
        }
        if (table[i][j] == -1)
        {
          tft.drawString("---", (width / columns) * (i + 0.5), 25 + j * LINEFEED, GFXFF);
        }
        else
        {
          if (strcmp(cols[i], "Nr.") == 0)
          {
            tft.drawString(String((int)table[i][j]), (width / columns) * (i + 0.5), 25 + j * LINEFEED, GFXFF);
          }
          else
          {
            tft.drawString(String(table[i][j]), (width / columns) * (i + 0.5), 25 + j * LINEFEED, GFXFF);
          }
        }
        tft.setTextColor(TFT_GREEN);
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

  void markCurrent(bool mark, TFT_eSPI tft)
  {
    tft.setTextDatum(TC_DATUM);
    int width = tft.width();
    if (half)
    {
      width /= 2;
    }
    if (mark)
    {
      tft.setTextColor(TFT_RED);
    }
    else
    {
      tft.setTextColor(TFT_GREEN);
    }

    if (table[selected_x][selected_y] == -1)
    {
      tft.drawString(String("---"), (width / columns) * (selected_x + 0.5), 25 + selected_y * LINEFEED, GFXFF);
    }
    else
    {
      tft.drawString(String(table[selected_x][selected_y]), (width / columns) * (selected_x + 0.5), 25 + selected_y * LINEFEED, GFXFF);
    }
    tft.setTextColor(TFT_GREEN);
  }

  void up(TFT_eSPI tft)
  {
    markCurrent(false, tft);

    selected_y--;
    if (selected_y < 0)
    {
      selected_y = rows - 1;
    }
    markCurrent(true, tft);
  }
  void down(TFT_eSPI tft)
  {
    markCurrent(false, tft);
    selected_y++;
    if (selected_y >= rows)
    {
      selected_y = 0;
    }
    markCurrent(true, tft);
  }
  void left(TFT_eSPI tft)
  {
    markCurrent(false, tft);
    selected_x--;
    if (selected_x < 0)
    {
      selected_x = columns - 1;
    }
    markCurrent(true, tft);
  }
  void right(TFT_eSPI tft)
  {
    markCurrent(false, tft);
    selected_x++;
    if (selected_x >= columns)
    {
      selected_x = 0;
    }
    markCurrent(true, tft);
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
  float **table;
  int rows, columns;
  char **cols;
  boolean *ints;
  bool half;
  int selected_x = -1;
  int selected_y = -1;
};

class Graph
{
public:
  Graph(int t_columns, int t_rows, boolean t_half)
  {
    half = t_half;
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

  void draw(float **table, TFT_eSPI tft)
  {
    drawColNull(table, 1, false, tft);
  }

  void drawColNull(float **table, int col, boolean startAtNull, TFT_eSPI tft)
  {

    int width = tft.width();
    int left;
    if (half)
    {
      left = tft.width() / 2;
      tft.fillRect(width / 2, 0, width / 2, tft.height() - 26, TFT_BLACK);
    }
    else
    {
      left = 0;
      tft.fillRect(0, 0, width, tft.height() - 26, TFT_BLACK);
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

      tft.drawLine(left + LINEFEED, 10, left + LINEFEED, tft.height() - 50, TFT_GREEN);
      tft.drawLine(left + LINEFEED, tft.height() - 50, tft.width() - 10, tft.height() - 50, TFT_GREEN);

      char buf[12];
      if (startAtNull)
      {
        sprintf(buf, "%.2f", 0.0);
      }
      else
      {
        sprintf(buf, "%.2f", min_y);
      }
      drawString90(buf, left, tft.height() - 50, tft);
      sprintf(buf, "%.2f", max_y);
      drawString90(buf, left, tft.textWidth(buf) + 10, tft);
      //vga.setCursor(left + vga.font->charHeight, 440);
      if (startAtNull)
      {
        sprintf(buf, "%.2f", 0.0);
      }
      else
      {
        sprintf(buf, "%.2f", min_x);
      }
      //vga.print(buf);
      tft.drawString(buf, left + 24, tft.height() - (26 + LINEFEED), GFXFF);
      sprintf(buf, "%.2f", max_x);
      tft.drawString(buf, tft.width() - (tft.textWidth(buf) + 10), tft.height() - (26 + LINEFEED), GFXFF);

      int tx;
      int ty;

      for (int i = 0; i < rows; i++)
      {
        if (table[0][i] != -1 && table[col][i] != -1)
        {
          if (startAtNull)
          {
            tx = mapf(table[0][i], 0, max_x, left + LINEFEED, tft.width() - 10);
          }
          else
          {
            tx = mapf(table[0][i], min_x, max_x, left + LINEFEED, tft.width() - 10);
          }
          if (startAtNull)
          {
            ty = tft.height() - mapf(table[col][i], 0, max_y, 50, tft.height() - 10);
          }
          else
          {
            ty = tft.height() - mapf(table[col][i], min_y, max_y, 50, tft.height() - 10);
          }
          tft.drawPixel(tx, ty, TFT_RED);
          tft.drawPixel(tx - 1, ty - 1, TFT_RED);
          tft.drawPixel(tx - 1, ty + 1, TFT_RED);
          tft.drawPixel(tx + 1, ty - 1, TFT_RED);
          tft.drawPixel(tx + 1, ty + 1, TFT_RED);
          tft.drawPixel(tx - 2, ty - 2, TFT_RED);
          tft.drawPixel(tx - 2, ty + 2, TFT_RED);
          tft.drawPixel(tx + 2, ty - 2, TFT_RED);
          tft.drawPixel(tx + 2, ty + 2, TFT_RED);
          Serial.print(1);
          Serial.print(" ");
          Serial.print(table[0][i]);
          Serial.print(" ");
          Serial.println(table[col][i]);
        }
      }
    }
  }

private:
  float **table;
  int rows, columns;
  bool half;
};

//Eingabe von Zahlen

void printMarker(const char *str, int marker, TFT_eSPI tft)
{
  tft.setTextDatum(ML_DATUM);
  int pos = tft.width() / 2 - tft.textWidth(str) / 2;

  int counter = 0;
  while (*str)
  {
    counter++;
    if (counter == marker)
    {
      tft.setTextColor(TFT_BLACK, TFT_GREEN);
      tft.drawString(String(*str), pos, tft.height() / 2 - 23, GFXFF);
    }
    else
    {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.drawString(String(*str), pos, tft.height() / 2 - 23, GFXFF);
    }
    pos += tft.textWidth(String(*str));
    str++;
  }
}

char *numberInput(char *input, TFT_eSPI tft)
{
  int pos = 1;
  int len = 0;
  char *output = strdup(input);
  while (*input)
  {
    input++;
    len++;
  }
  tft.fillRect(0, 0, tft.width(), tft.height() - 26, TFT_BLACK);
  printMarker(output, pos, tft);
  drawLegend("+", "-", "<", ">", "Bestätigen", tft);
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
      tft.fillRect(0, tft.height() / 2 - 43, tft.width(), LINEFEED * 2, TFT_BLACK);
      printMarker(output, pos, tft);
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
      tft.fillRect(0, tft.height() / 2 - 43, tft.width(), LINEFEED * 2, TFT_BLACK);
      printMarker(output, pos, tft);
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

      tft.fillRect(0, tft.height() / 2 - 43, tft.width(), LINEFEED * 2, TFT_BLACK);
      printMarker(output, pos, tft);
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

      tft.fillRect(0, tft.height() / 2 - 43, tft.width(), LINEFEED * 2, TFT_BLACK);
      printMarker(output, pos, tft);
      delay(200);
    }
  }
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  return output;
}

void jpegRender(int xpos, int ypos, TFT_eSPI tft)
{

  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  bool swapBytes = tft.getSwapBytes();
  tft.setSwapBytes(true);

  uint16_t w_mod = max_x % mcu_w;
  uint16_t h_mod = max_y % mcu_h;
  uint32_t min_w = min(mcu_w, w_mod);
  uint32_t min_h = min(mcu_h, h_mod);

  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  max_x += xpos;
  max_y += ypos;

  while (JpegDec.read())
  {                        // While there is more data in the file
    pImg = JpegDec.pImage; // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)

    // Calculate coordinates of top left corner of current MCU
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x)
      win_w = mcu_w;
    else
      win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y)
      win_h = mcu_h;
    else
      win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // draw image MCU block only if it will fit on the screen
    if ((mcu_x + win_w) <= tft.width() && (mcu_y + win_h) <= tft.height())
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    else if ((mcu_y + win_h) >= tft.height())
      JpegDec.abort(); // Image has run off bottom of screen so abort decoding
  }

  tft.setSwapBytes(swapBytes);
}

void drawJpeg(const char *filename, int xpos, int ypos, TFT_eSPI tft)
{

  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  File jpegFile = SPIFFS.open(filename, FILE_READ); // or, file handle reference for SD library

  if (!jpegFile)
  {
    Serial.println("File not found!");
    return;
  }
  boolean decoded = JpegDec.decodeSdFile(jpegFile); // Pass the SD file handle to the decoder,

  if (decoded)
  {

    jpegRender(xpos, ypos, tft);
  }
  else
  {
    Serial.println("Jpeg file format not supported!");
  }
}

void showtext(const char *filename, TFT_eSPI tft)
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open(filename);

  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  drawLegend("Beenden", "", "", "", "Weiter", tft);
  while (file.available())
  {
    tft.fillRect(0, 0, 480, tft.height() - 26, TFT_BLACK);
    tft.setTextFont(1);
    tft.setCursor(0, 10);
    while (tft.getCursorY() < 280)
    {
      if (file.available())
      {
        tft.write(file.read());
      }
      else
      {
        break;
      }
    }
    tft.setCursor(370, 310);
    tft.setFreeFont(SANSBOLD9);
    while (digitalRead(SW5) == HIGH)
    {
      if (digitalRead(SW1) == LOW)
      {
        tft.fillScreen(TFT_BLACK);
        return;
      }
    }
  }
  tft.fillScreen(TFT_BLACK);
  file.close();
}
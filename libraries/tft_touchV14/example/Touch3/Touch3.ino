/*************** Avi Hayun 27/10/18 ***************/

#include "TFT9341Touch.h"

tft9341touch LcdTouch(10, 9, 7, 2); //cs, dc ,tcs, tirq

void setup() {
  LcdTouch.begin();
  LcdTouch.clearButton();
  LcdTouch.setRotation(0);//0,1,2,3
  LcdTouch.setTextSize (2);
  LcdTouch.setCursor (40, 40);
  LcdTouch.set(3780, 372, 489, 3811); //Calibration

  screenMain();
}

void loop() {
  uint16_t x, y;
  String str;
  
  if (LcdTouch.touched())
  {
    LcdTouch.readTouch();
    x = LcdTouch.xTouch;
    y = LcdTouch.yTouch;

    int ButtonNum = LcdTouch.ButtonTouch(x, y);
    str="x=" + String(x) + " y=" + String(y) + " Bu=" + String(ButtonNum);
    LcdTouch.print (25, 220, str, 2, BLACK, CYAN);
    LcdTouch.fillCircle(x, y, 3, WHITE);

    if (ButtonNum == 1) 
    {
      screen1();
      screenMain();
    }
    else if (ButtonNum == 2) 
    {
      screen2();
      screenMain();
    }
  }
}


void screenMain() {
  LcdTouch.fillScreen (BLACK);
  LcdTouch.printheb(120, 160, " אבי חיון ", 3, WHITE);
  LcdTouch.drawButton(1, 20,  20, 90, 50, 10, RED, WHITE, "EXAM1", 2); // NumButton, x, y, width, height, r, Color, textcolor, label, textsize);
  LcdTouch.drawButton(2, 20,  100, 90, 50, 10, BLUE, WHITE, "EXAM2", 2);
}


void screen1() {
  LcdTouch.fillScreen (BLACK);
  LcdTouch.setTextColor(BLACK, CYAN);
  LcdTouch.setTextSize (3);
  LcdTouch.setCursor (25, 30);
  LcdTouch.println ("OK1");
  LcdTouch.setTextSize (1);
  LcdTouch.print ("Press Keyboard to return");
  LcdTouch.drawLine(20, 180, 170, 70, RED); // x0, y0, x1, y1, color
  LcdTouch.drawHLine (30, 100, 120, GREEN); // x, y, width
  LcdTouch.drawVLine (100, 100, 70, YELLOW); // x, y, height
  LcdTouch.drawRect (150, 150, 40, 30, BLUE); // x, y,width,height  
  LcdTouch.drawTriangle(180, 100, 210, 150, 140, 235, CYAN); // x0, y0, x1, y1, x2, y2, color
  LcdTouch.drawCircle(120, 120, 30, WHITE); // x, y, r, color
  while (!LcdTouch.touched());
}


void screen2() {
  LcdTouch.fillScreen (RED);
  LcdTouch.setTextColor(BLACK, WHITE);
  LcdTouch.setTextSize (4);
  LcdTouch.setCursor (25, 30);
  LcdTouch.println ("OK2");
  LcdTouch.setTextSize (1);
  LcdTouch.print ("Press Keyboard to return");
  LcdTouch.fillRect(10, 150, 80, 50, BLUE); // x, y,width,height
  LcdTouch.fillTriangle(180, 100, 210, 150, 140, 235, CYAN); // x0, y0, x1, y1, x2, y2, color
  LcdTouch.fillCircle(120, 120, 30, WHITE); // x, y, r, color

  while (!LcdTouch.touched());
}

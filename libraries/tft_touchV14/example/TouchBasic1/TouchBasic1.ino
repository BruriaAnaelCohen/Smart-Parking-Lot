
#include "TFT9341Touch.h"

tft9341touch LcdTouch(10, 9, 7, 2); //cs, dc ,tcs, tirq

void setup() {
  LcdTouch.begin();
  LcdTouch.clearButton();
  LcdTouch.setRotation(0);//0,1,2,3
  LcdTouch.setTextSize (2);
  LcdTouch.setCursor (40, 40);
  LcdTouch.set(3780, 372, 489, 3811);
  fun();
}

void loop() {
  uint16_t x, y;
  if (LcdTouch.touched())
  {
    LcdTouch.readTouch();
    x = LcdTouch.xTouch;
    y = LcdTouch.yTouch;

    int ButtonNum = LcdTouch.ButtonTouch(x, y);
    
    if (ButtonNum == 1) 
    {
      LcdTouch.fillScreen (BLUE);
     LcdTouch.print (5,100,"Press Keyboard to return",2,YELLOW);
      while (!LcdTouch.touched());
      fun();
    }
  }
}


void fun(){
  LcdTouch.fillScreen (BLACK);
  LcdTouch.printheb(120, 160, " אבי חיון ", 3, WHITE);
  LcdTouch.drawButton(1, 20,  20, 90, 50, 10, RED, WHITE, "EXAM1", 2); // NumButton, x, y, width, height, r, Color, textcolor, label, textsize);
}




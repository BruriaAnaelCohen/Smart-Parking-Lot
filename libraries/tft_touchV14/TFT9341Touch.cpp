/*  last update 27/07/18 */
/*  Avi Hayun */


#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "tft9341touch.h"
#include "glcdfonth.h"
#include "registers.h"
#include <SPI.h>
#include "Arduino.h"

tft9341touch::tft9341touch(int8_t CS, int8_t DC, int8_t Tcs, int8_t Tirq)
{
	//pin lcd
	_cs   = CS;
	_dc   = DC;
	
	// pin touch
	_Tcs=Tcs;
	_Tirq=Tirq;
	

	_x = 0;
	_y = 0;
	_width    = WIDTH;
	_height   = HEIGHT;
	_fontSize = 1;
	_rotation  = 0;
	_color = BLACK;
	_background = WHITE;
	numbera = 0;
	numberb = 0;
	number1 = 1;
	number2 = 64;
}

tft9341touch::tft9341touch(int8_t CS, int8_t DC)
{
	//pin lcd
	_cs   = CS;
	_dc   = DC;

	_x = 0;
	_y = 0;
	_width    = WIDTH;
	_height   = HEIGHT;
	_fontSize = 1;
	_rotation  = 0;
	_color = BLACK;
	_background = WHITE;
	numbera = 0;
	numberb = 0;
	number1 = 1;
	number2 = 64;
}

//send data 8-bit to spi
void tft9341touch::writeData(uint8_t data8)
{
	//set clockDivider to SPI_CLOCK_DIV2 by default which is 8MHz
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	_SPIF=0;
	
	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, LOW);
	
	SPDR = data8;
	while(!_SPIF);

	digitalWrite(_cs, HIGH);
}

//write command 8-bit to spi
void tft9341touch::writeCommand(uint8_t cmd)
{
	//set clockDivider to SPI_CLOCK_DIV2 by default which is 8MHz
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	_SPIF=0;

	digitalWrite(_dc, LOW);
	digitalWrite(_cs, LOW);

	SPDR = cmd;
	while(!_SPIF);

	digitalWrite(_cs, HIGH);
}


void tft9341touch::pushColor(uint16_t color) {
	
	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, LOW);
	

	SPI.transfer(color >> 8);
	SPI.transfer(color);

	
	digitalWrite(_cs, HIGH);
	
}



void tft9341touch::begin()
{
	// sets ports as outputs excepts MISO
	// set ports
	
	pinMode(_Tcs, OUTPUT);
	pinMode(_Tirq, INPUT_PULLUP);
	digitalWrite(_Tcs,HIGH);
	
	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	
	SPI.begin(); 
	
	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, HIGH);
	
	//reset LCD
	//O_RES = 0; //RES=0
//	_delay_ms(10);
//	O_RES = 1; //RES=1
//	_delay_ms(10);

	writeCommand(0x11);
	_delay_ms(10);
	writeCommand(0xCF);
	writeData(0x00);
	writeData(0xc3);
	writeData(0X30);

	writeCommand(0xED);
	writeData(0x64);
	writeData(0x03);
	writeData(0X12);
	writeData(0X81);

	writeCommand(0xE8);
	writeData(0x85);
	writeData(0x10);
	writeData(0x79);

	writeCommand(0xCB);
	writeData(0x39);
	writeData(0x2C);
	writeData(0x00);
	writeData(0x34);
	writeData(0x02);

	writeCommand(0xF7);
	writeData(0x20);

	writeCommand(0xEA);
	writeData(0x00);
	writeData(0x00);

	writeCommand(0xC0);    //Power control
	writeData(0x22);   //VRH[5:0]

	writeCommand(0xC1);    //Power control
	writeData(0x11);   //SAP[2:0];BT[3:0]

	writeCommand(0xC5);    //VCM control
	writeData(0x3d);
	writeData(0x20);

	writeCommand(0xC7);    //VCM control2
	writeData(0xAA); //0xB0

	writeCommand(0x36);    // Memory Access Control
	writeData(0x68);

	writeCommand(0x3A);
	writeData(0x55);

	writeCommand(0xB1);
	writeData(0x00);
	writeData(0x13);

	writeCommand(0xB6);    // Display Function Control
	writeData(0x0A);
	writeData(0xA2);

	writeCommand(0xF6);
	writeData(0x01);
	writeData(0x30);

	writeCommand(0xF2);    // 3Gamma Function Disable
	writeData(0x00);

	writeCommand(0x26);    //Gamma curve selected
	writeData(0x01);

	writeCommand(0xE0);    //Set Gamma
	writeData(0x0F);
	writeData(0x3F);
	writeData(0x2F);
	writeData(0x0C);
	writeData(0x10);
	writeData(0x0A);
	writeData(0x53);
	writeData(0XD5);
	writeData(0x40);
	writeData(0x0A);
	writeData(0x13);
	writeData(0x03);
	writeData(0x08);
	writeData(0x03);
	writeData(0x00);

	writeCommand(0XE1);    //Set Gamma
	writeData(0x00);
	writeData(0x00);
	writeData(0x10);
	writeData(0x03);
	writeData(0x0F);
	writeData(0x05);
	writeData(0x2C);
	writeData(0xA2);
	writeData(0x3F);
	writeData(0x05);
	writeData(0x0E);
	writeData(0x0C);
	writeData(0x37);
	writeData(0x3C);
	writeData(0x0F);

	writeCommand(0x11);    //Exit Sleep
	_delay_ms(20);
	writeCommand(0x29);    //Display on
	_delay_ms(100);
	fillScreen (BLACK);
    clearButton();

    
	if (touched()) {
    print("press to set");
    cal();
    while (1);
  }
}

// all display one color
void tft9341touch::fillScreen (uint16_t color)
{
	int16_t i,j;
	
	
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	_x = 0;
	_y = 0;
	sector(0,_width-1,0,_height-1);//320x240
	
	_SPIF=0;

	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, LOW);
	
	for (i=0;i<_height;i++)
	for (j=0;j<_width;j++){
		//SPI.transfer(color>>8);
		//SPI.transfer( color);
		
		SPDR = color>>8;
		while(!_SPIF);
		_SPIF=0;
		SPDR =color;
		while(!_SPIF);
	}
	
	digitalWrite(_cs, HIGH);
}

void tft9341touch::sector(int16_t x0, int16_t x1, int16_t y0, int16_t y1)
{
	writeCommand(CASET); // Column addr set
	writeData(x0 >> 8);
	writeData(x0 & 0xFF);     // XSTART
	writeData(x1 >> 8);
	writeData(x1 & 0xFF);     // XEND

	writeCommand(PASET); // Row addr set
	writeData(y0>>8);
	writeData(y0);     // YSTART
	writeData(y1>>8);
	writeData(y1);     // YEND

	writeCommand(RAMWR); // write to RAM
}

void tft9341touch::setAddrWindow(int16_t x0, int16_t x1, int16_t y0, int16_t y1)
{
sector( x0, x1,  y0, y1);	
}

// Draw a character
void tft9341touch::putChar(uint8_t c)
{
	int8_t i,j;
	uint8_t line;
	
	if ((_x >= _width) || (_y >= _height) || ((_x + 6 * _fontSize - 1) < 0) || ((_y + 8 * _fontSize - 1) < 0))
	return;

	for (i=0; i<6; i++ )
	{
		if (i == 5)
		line = 0x0;
		else
		line =pgm_read_byte(font+(c*5)+i);
		//line=pgm_read_byte(font+(144*5)+i);
		for (j = 0; j<8; j++)
		{
			if (line & 0x1)
			{
				if (_fontSize == 1) // default size
				drawPixel(_x+i, _y+j,_color);
				else
				fillRect(_x+(i*_fontSize), _y+(j*_fontSize), _fontSize, _fontSize,_color);
			}
			else if (_background != _color)
			{
				//switchColors();
				if (_fontSize == 1) // default size
				drawPixel(_x+i, _y+j,_background);//_color);
				else  // big size
				fillRect(_x+i*_fontSize, _y+j*_fontSize, _fontSize, _fontSize,_background);//_color);
				//switchColors();
			}
			line >>= 1;
		}
	}
	_x += 6 * _fontSize;
}


void tft9341touch::write (char c)
{
	putChar(c);
}

void tft9341touch::writeln (char c)
{
	write (c);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341touch::print (int16_t num, uint8_t radix)
{
	char str[17];
	itoa (num, str, radix);
	print (str);
}

void tft9341touch::println (int16_t num, uint8_t radix)
{
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341touch::println ()
{
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341touch::print (uint16_t num, uint8_t radix)
{
	char str[16];

	itoa (num, str, radix);
	print (str);
}

void tft9341touch::println (uint16_t num, uint8_t radix)
{
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341touch::print (uint8_t num, uint8_t radix)
{
	char str[8];

	itoa (num, str, radix);
	print (str);
}

void tft9341touch::println (uint8_t num, uint8_t radix)
{
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341touch::print (int8_t num, uint8_t radix)
{
	char str[8];

	itoa (num, str, radix);
	print (str);
}

void tft9341touch::println (int8_t num, uint8_t radix)
{
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void tft9341touch::print (char *str)
{
	uint8_t i,temp;
	for (i=0;str[i];i++){
		temp=str[i];
		if(temp <0xb0) 	write(temp);
	}
}


void tft9341touch::print (String txtMsg)
{
	int8_t i,temp;

	for(i=0; i<txtMsg.length(); i++){
		 temp=txtMsg[i];
		if(temp <0xb0) write(temp);
	}
}


void tft9341touch::println (String txtMsg)
{
	print (txtMsg);
	_x = 0;
	_y += 8 * _fontSize +2;
}

/*
void tft9341touch::printheb (String txtMsg)
{
	int8_t i=8;//txtMsg.length()-1;
    uint8_t temp;
	while(i>=0){
		 temp=txtMsg[i];
		if(temp < 0xb0) write(temp); //write(144);
		i--;
	}
	
}*/

void tft9341touch::printheb (char *str)
{
	int8_t i=0;
	while(str[i++]);
   i--;
  
	while(i>=0){
		uint8_t temp=str[i];
		if(temp <0xb0) write(temp);
		i--;
	}
	
	
}

void tft9341touch::printhebln (char *str)
{
	printheb (str);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void tft9341touch::println (char *str)
{
	print (str);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341touch::print (double fnum,uint8_t s)
{
	int16_t num1,num2,e;
	double fnum1;
	if (s>5) s=5;
	e = pow (10,s);
	num1 = trunc(fnum);
	fnum1 = fnum - num1;
	num2 = abs(round(fnum1*e));
	
	print (num1);
	write ('.');
	print (num2);
}

void tft9341touch::println (double fnum, uint8_t s)
{
	print (fnum,s);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void tft9341touch::print (uint16_t num)
{
	char str[8];

	ltoa (num, str, DEC);
	print (str);
}


void tft9341touch::println (uint16_t num)
{
	print (num);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341touch::print (uint16_t x,uint16_t y,char *str,uint16_t fontSize, uint16_t fColor)
{
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor);
	print (str);
}

void tft9341touch::print (uint16_t x,uint16_t y,char *str,uint16_t fontSize, uint16_t fColor,uint16_t bColor)
{
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor,bColor);
	print (str);
}
/////////////////
void tft9341touch::print (uint16_t x,uint16_t y,String txtMsg,uint16_t fontSize, uint16_t fColor)
{
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor);
	print (txtMsg);
}

void tft9341touch::print (uint16_t x,uint16_t y,String txtMsg,uint16_t fontSize, uint16_t fColor,uint16_t bColor)
{
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor,bColor);
	print (txtMsg);
}
//////////////////////////////////


void tft9341touch::printheb (uint16_t x,uint16_t y,char *str,uint16_t fontSize, uint16_t fColor)
{
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor);
	printheb (str);
}

void tft9341touch::printheb (uint16_t x,uint16_t y,char *str,uint16_t fontSize, uint16_t fColor,uint16_t bColor)
{
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor,bColor);
	printheb (str);
}


void tft9341touch::drawPixel (int16_t x, int16_t y,uint16_t color)
{
	sector(x,x,y,y);
	_SPIF=0;
	
	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, LOW);
	
	SPDR = color>>8;
	while(!_SPIF);
	_SPIF=0;
	SPDR = color;
	while(!_SPIF);
	digitalWrite(_cs, HIGH);
}

void tft9341touch::drawHLine (int16_t x, int16_t y, int16_t w,uint16_t color)
{
	sector(x,x+w-1,y,y);
	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, LOW);
	
	while (w--){
		_SPIF=0;
		
		SPDR = color>>8;
		while(!_SPIF);
		_SPIF=0;
		SPDR = color;
		while(!_SPIF);
	}
	digitalWrite(_cs, HIGH);
}

void tft9341touch::drawFastHLine (int16_t x, int16_t y, int16_t w,uint16_t color)
{
drawHLine ( x,  y,  w, color);	
}


void tft9341touch::drawVLine (int16_t x, int16_t y, int16_t h,uint16_t color)
{
	sector(x,x,y,y+h-1);
	_SPIF=0;
	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, LOW);
	
	while (h--){
		SPDR = color>>8;
		while(!_SPIF);
		_SPIF=0;
		SPDR = color;
		while(!_SPIF);
	}
	digitalWrite(_cs, HIGH);
}


void tft9341touch::drawFastVLine (int16_t x, int16_t y, int16_t h,uint16_t color)
{
drawVLine ( x,  y,  h,color);	
}

void tft9341touch::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color) {
	int16_t steep = ABS(y1 - y0) > ABS(x1 - x0);
	if (steep) {
		SWAP(x0, y0);
		SWAP(x1, y1);
	}

	if (x0 > x1) {
		SWAP(x0, x1);
		SWAP(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = ABS(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
		} else {
		ystep = -1;
	}

	for (; x0<=x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
			} else {
			drawPixel(x0, y0,color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void tft9341touch::drawLinePolar (int16_t x0, int16_t y0, int16_t r, int16_t angle,uint16_t color){
	float x,y;
	uint16_t x1;
	uint16_t y1;
	
	x = x0 + r*cos (angle*M_PI/180);
	y = y0 + r*sin (angle*M_PI/180);
	
	x1 = round (x);
	y1 = round (y);
	drawLine (x0,y0,x1,y1,color);
}

void tft9341touch::drawRect ( int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
	drawHLine(x,y,w,color);
	drawHLine(x,y+h,w,color);
	drawVLine(x,y,h,color);
	drawVLine(x+w,y,h,color);
}

// Draw a triangle
void tft9341touch::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint16_t color) {
	drawLine(x0, y0, x1, y1,color);
	drawLine(x1, y1, x2, y2,color);
	drawLine(x2, y2, x0, y0,color);
}

void tft9341touch::drawCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0  , y0+r,color);
	drawPixel(x0  , y0-r,color);
	drawPixel(x0+r, y0,color);
	drawPixel(x0-r, y0,color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		
		drawPixel(x0 + x, y0 + y,color);
		drawPixel(x0 - x, y0 + y,color);
		drawPixel(x0 + x, y0 - y,color);
		drawPixel(x0 - x, y0 - y,color);
		drawPixel(x0 + y, y0 + x,color);
		drawPixel(x0 - y, y0 + x,color);
		drawPixel(x0 + y, y0 - x,color);
		drawPixel(x0 - y, y0 - x,color);
	}
}

void tft9341touch::drawCircleHelper( int16_t x0, int16_t y0, int16_t r, int8_t cornername,uint16_t color) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y,color);
			drawPixel(x0 + y, y0 + x,color);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y,color);
			drawPixel(x0 + y, y0 - x,color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x,color);
			drawPixel(x0 - x, y0 + y,color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x,color);
			drawPixel(x0 - x, y0 - y,color);
		}
	}
}

// Draw a rounded rectangle
void tft9341touch::drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r,uint16_t color) {
	// smarter version
	drawHLine(x+r  , y    , w-2*r,color); // Top
	drawHLine(x+r  , y+h-1, w-2*r,color); // Bottom
	drawVLine(x    , y+r  , h-2*r,color); // Left
	drawVLine(x+w-1, y+r  , h-2*r,color); // Right
	// draw four corners
	drawCircleHelper(x+r    , y+r    , r, 1,color);
	drawCircleHelper(x+w-r-1, y+r    , r, 2,color);
	drawCircleHelper(x+w-r-1, y+h-r-1, r, 4,color);
	drawCircleHelper(x+r    , y+h-r-1, r, 8,color);
}

void tft9341touch::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color)
{
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	sector(x,x+w-1,y,y+h-1);
	_SPIF=0;
	//O_DC = 1;	//DC=1;
	digitalWrite(_dc, HIGH);
	//O_SS = 0;	//SS=0;
	digitalWrite(_cs, LOW);
	
	for(y=h; y>0; y--)
	for(x=w; x>0; x--){
		
		
		SPDR = color>>8;
		while(!_SPIF);
		_SPIF=0;
		SPDR = color;
		while(!_SPIF);
	}
	digitalWrite(_cs, HIGH);
	//writeData2bytes(color>>8,color);
}


// Fill a triangle
void tft9341touch::fillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint16_t color)
{

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		SWAP(y0, y1); SWAP(x0, x1);
	}
	if (y1 > y2) {
		SWAP(y2, y1); SWAP(x2, x1);
	}
	if (y0 > y1) {
		SWAP(y0, y1); SWAP(x0, x1);
	}

	if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if(x1 < a)      a = x1;
		else if(x1 > b) b = x1;
		if(x2 < a)      a = x2;
		else if(x2 > b) b = x2;
		drawHLine(a, y0, b-a+1,color);
		return;
	}

	int16_t
	dx01 = x1 - x0,
	dy01 = y1 - y0,
	dx02 = x2 - x0,
	dy02 = y2 - y0,
	dx12 = x2 - x1,
	dy12 = y2 - y1,
	sa   = 0,
	sb   = 0;


	if(y1 == y2) last = y1;   // Include y1 scanline
	else         last = y1-1; // Skip it

	for(y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if(a > b) SWAP(a,b);
		drawHLine(a, y, b-a+1,color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if(a > b) SWAP(a,b);
		drawHLine(a, y, b-a+1,color);
	}
}

void tft9341touch::fillCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color)
{
	drawVLine(x0, y0-r, 2*r+1,color);
	fillCircleHelper(x0, y0, r, 3, 0,color);
}

void tft9341touch::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t cornername, int16_t delta,uint16_t color)
{

	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawVLine(x0+x, y0-y, 2*y+1+delta,color);
			drawVLine(x0+y, y0-x, 2*x+1+delta,color);
		}
		if (cornername & 0x2) {
			drawVLine(x0-x, y0-y, 2*y+1+delta,color);
			drawVLine(x0-y, y0-x, 2*x+1+delta,color);
		}
	}
}

// Fill a rounded rectangle
void tft9341touch::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,uint16_t color) {
	// smarter version
	fillRect(x+r, y, w-2*r, h,color);

	// draw four corners
	fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1,color);
	fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1,color);
}

void tft9341touch::invertDisplay(uint8_t i) {
	writeCommand(i ? INVON : INVOFF);
}


void tft9341touch::setRotation(uint8_t m) {

	writeCommand(MADCTL);
	_rotation = m%4;
	switch (_rotation) {
		case 0:
		writeData(right2left);
		_height = HEIGHT;
		_width = WIDTH;
		_x = 0;
		_y = 0;
		break;
		case 1:
		writeData(down2up);
		_height = WIDTH;
		_width = HEIGHT;
		_x = 0;
		_y = 0;
		break;
		case 2:
		writeData(left2right);
		_height = HEIGHT;
		_width = WIDTH;
		_x = 0;
		_y = 0;
		break;
		case 3:
		writeData(up2down);
		_height = WIDTH;
		_width = HEIGHT;
		_x = 0;
		_y = 0;
		break;
	}
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t tft9341touch::color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/*
uint16_t tft9341touch::color565(uint32_t color)
{
	uint8_t r,g,b;
	
	b = color & 0xFF;
	g = (color >> 8) & 0xFF;
	r = (color >> 16) & 0xFF;
	
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}*/

void tft9341touch::putRgbColorPixel(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t colorPixel;
	colorPixel = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	
	_SPIF=0;

	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, LOW);
	
	SPDR = colorPixel>>8;
	while(!_SPIF);
	_SPIF=0;
	SPDR = colorPixel;
	while(!_SPIF);
	digitalWrite(_cs, HIGH);
}

void tft9341touch::startScrollingRolling()
{
	writeCommand(0x33);
	writeData(0x00);
	writeData(0x00);
	writeData(0x01);
	writeData(0x2c);
	writeData(0x00);
	writeData(0x14);
}

void tft9341touch::endScrollingRolling (uint8_t Direction)
{
	if( Direction == 2)
	{
		writeCommand(0x37);
		writeData(numbera);
		writeData(numberb);

		if (numbera == 0)
		{
			if (numberb < 255 )
			numberb++;
			else
			{
				numbera = 1;
				numberb = 0;
			}
		}
		else
		{
			if	(numberb < 64)
			numberb++;
			else
			{
				numberb=0;
				numbera=0;
			}
		}
	}
	
	if( Direction == 1)
	{
		writeCommand(0x37);
		writeData(number1);
		writeData(number2);

		if(number1 == 1)
		{
			if(number2 > 0)
			number2--;
			else
			{
				number2 = 255;
				number1 = 0;
			}
		}

		else if (number2> 0)
		number2--;
		else
		{
			number2 = 64 ;
			number1 = 1;
		}
	}
}



/*  touch   */



bool tft9341touch::touched()
{
	uint8_t data1, data2;
	if (!digitalRead(_Tirq))
	{
		_delay_ms(10);
		//readxy();
		//while(!digitalRead(_Tirq));
		//_delay_ms(10);
		return true;
	}
	else return false;
}

uint16_t tft9341touch::getToucX()
{
	uint8_t data1, data2;
	uint16_t dx;
	SPI.setClockDivider(SPI_CLOCK_DIV16);
	digitalWrite(_Tcs,LOW);//T_SS = 0;
	SPI.transfer(0xD0);////
	data1 = SPI.transfer(0x00);
	data2 = SPI.transfer(0x00);
	dx = (((data1<<8)+data2)>>3) &0xFFF;
	digitalWrite(_Tcs,HIGH);//T_SS = 1;
	return (dx);
}

uint16_t tft9341touch::getToucY()
{
	uint8_t data1, data2;
	uint16_t dy;
	SPI.setClockDivider(SPI_CLOCK_DIV16);
	digitalWrite(_Tcs,LOW);//T_SS = 0;
	SPI.transfer(0x90);/////
	data1 = SPI.transfer(0x00);
	data2 = SPI.transfer(0x00);
	dy = (((data1<<8)+data2)>>3) &0xFFF;
	digitalWrite(_Tcs,HIGH);//T_SS = 1;
	return (dy);
}



void tft9341touch::readTouch()
{

	_x = map(getToucX(), x1, x2, 10, 230);
	_y = map(getToucY(), y1, y2, 10, 310);

	int temp = _x;
	

	if (_rotation == 1){
	}
	else if (_rotation == 2){
		_x = _y;
		_y = 239 - temp;
	}
	else if (_rotation == 0){
		_x = 319 - _y;
		_y = temp;
	}

	else if (_rotation == 3){
		_x = 239 - _x;
		_y = 319 - _y;
	}



	if (_x<0) _x = 0;

	if (_x>(_width - 1)) _x = _width - 1;

	if (_y<0) _y = 0;

	if (_y>(_height - 1)) _y = _height - 1;
	
	xTouch=_x; yTouch=_y;
}





void tft9341touch::cal()
{
	setRotation(1);
	
	
	while (!digitalRead(_Tirq));
	delay(30);
	
	fillCircle(10, 10, 3, WHITE);

	while (digitalRead(_Tirq));
	
	delay(30);
	
	x1 =getToucX();
	y1 = getToucY();
	while (!digitalRead(_Tirq));

	fillCircle(10, 10, 3, BLACK);
	delay(30);

	fillCircle(_width - 10, _height - 10, 3, WHITE);
	
	while (digitalRead(_Tirq));
	delay(30);
	
	x2 =getToucX();
	y2 =  getToucY();
	while (!digitalRead(_Tirq));
	fillCircle(_width - 10, _height - 10, 3, BLACK);
	delay(30);


	setCursor(10, 10);
	setTextColor(WHITE);  setTextSize(2);
	
	print(x1);print(" ");  print(x2);print(" "); print(y1);print(" "); println(y2);
	while (digitalRead(_Tirq));
	delay(30);

}


void tft9341touch::set(uint16_t xs1, uint16_t xs2, uint16_t ys1, uint16_t ys2)
{
	x1 = xs1; x2 = xs2; y1 = ys1; y2 = ys2;

}


#define nB 20
int  n[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void tft9341touch::drawButton(int8_t NumButton,int16_t x, int16_t y, int16_t w,int16_t h, int16_t r, uint16_t Color,uint16_t textcolor,char *label,int8_t textsize)
{
	int16_t x1,y1;
	int16_t len=strlen((const char *)label);
	
	fillRoundRect( x, y,  w, h,  r,  Color);
	
	
	
	x1=x+(w-(len*6*textsize))/2;
	y1=y+(h/2)-(8*textsize)/2;
	print(x1,y1,label,textsize,textcolor,Color);
	
	if(NumButton< (nB+1) && NumButton>0)
	{
		n[NumButton-1]=1;
		LCD_Button[NumButton-1].x=x;
		LCD_Button[NumButton-1].y=y;
		LCD_Button[NumButton-1].w=w;
		LCD_Button[NumButton-1].h=h;
		//LCD_Button[NumButton-1].Color=Color;
		//LCD_Button[NumButton-1].fontColor=textcolor;
		//LCD_Button[NumButton-1].fontSize=textsize;
		
	}
}

void tft9341touch::clearButton(void)
{
int i;
	for(i=0;i<nB;i++)
	n[i]=0;
}


int8_t tft9341touch::ButtonTouch(int16_t x,int16_t y)
{
	int8_t i;
	for(i=0;i<nB;i++)
	{
		if(n[i])
		if(x>LCD_Button[i].x && x< (LCD_Button[i].x +LCD_Button[i].w) && y>LCD_Button[i].y && y<(LCD_Button[i].y +LCD_Button[i].h))
		return i+1;
		
	}
	return 0;
}






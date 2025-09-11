/*************** Proud to produce: Smart Parking-lot *********/

#include "TFT9341Touch.h"
#include "RTClib.h"
#include "Servo.h"

tft9341touch LcdTouch(10, 9, 7, 2); //cs, dc ,tcs, tirq
RTC_DS1307 rtc;
Servo servo;

#define BLACK   0x0000
#define GRAY    0x8410
#define BLUE    0x001F
#define RED     0xF800
#define PINK    0xF81F
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define PURPLE  0x7194
#define ORANGE  0xFE00
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

unsigned long interval = 1000;
unsigned long ms_start = 0;
unsigned long ms_prev_read = 0;

static const char  daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const int PARKINGS = 2;
const int firstLedPIN = 3;
const int ldrPin[PARKINGS] = {A0, A1};
const double pricePerHour = 35;

int ldrValueArr[PARKINGS];
const int thresholdLDRValue = 300;
const int thresholdGPValue = 6;

bool isFreeParking[PARKINGS];
int ledPin[PARKINGS][2];
DateTime timer[PARKINGS];

double voltage = 0;
double distance = 0;
const int gpPin = A2;

bool potentialCarWaitAtEntry= true;

enum Mode {
  off, 
  freeParking, 
  occupiedParking
};

enum Btn {
  openSmartParkingLotBtn,
  payBtn,
  closeBtn
};

Mode mode;
const int servoPin = 8;
const int degree = 22;
double totalProfit = 0;

void systemInitialization() {
  LcdTouch.begin();                   // Init LcdTouch Settings
  LcdTouch.setRotation(0);
  LcdTouch.set(3780, 372, 489, 3811); 
  
  setRTC();                           // Init curr RTC time
  
  servo.attach(servoPin);             // Init SERVO Pin
  servo.write(degree);
  
  pinMode(gpPin, INPUT);
  
  for(int i = 0; i < PARKINGS; ++i) { 
    ldrValueArr[i] = 0;
    isFreeParking[i] = true;
  }
  
  for(int i =0; i < PARKINGS; ++i) {   // Init LED Pins
    ledPin[i][0] = firstLedPIN + i*2;
    ledPin[i][1] = firstLedPIN + 1 + i*2;
  }
  
  for(int i = 0; i < sizeof(ledPin) / sizeof(int); ++i) {
    for(int j = 0; j < sizeof(ledPin[i]); ++j) {
      pinMode(ledPin[i][j], OUTPUT);
    }
  }
}

void setRTC() {
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    LcdTouch.println("Couldn't find RTC");
    while(1);
  }

  systemInitialization(); 
  screenMain();
}

void loop()
{
  ms_start = millis();

  if(ms_start - ms_prev_read> interval * 0.5) { // Checks for change every 500 millisecond 
    ms_prev_read = ms_start;

    uint16_t x, y;

    if (LcdTouch.touched()) {
      LcdTouch.readTouch();
      x =LcdTouch.xTouch;
      y = LcdTouch.yTouch;
      
      int ButtonNum = LcdTouch.ButtonTouch(x,y);
      
      switch(ButtonNum) {
        case openSmartParkingLotBtn: {
          openParkingLot();
          screenActive();
          break;
        }

        case payBtn: {
          screenActive();
          openGate();
          break;
        }

        case closeBtn: {
          screenClose();
          screenMain();
          break;
        }
      }
    }

    checkForChange();
  }
}

void checkForChange() {
  for(int i = 0; i < PARKINGS; ++i) {
    ldrValueArr[i] = analogRead(ldrPin[i]);
    
    Serial.print("Parking ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(ldrValueArr[i]);
    Serial.print("isFreeParking: ");
    Serial.println(isFreeParking[i]);
  }

  for(int i=0; i<PARKINGS; ++i) {
    if(ldrValueArr[i] < thresholdLDRValue && isFreeParking[i]) {     // Car is parking
      mode = occupiedParking;
      changeParkingMode(i);
      
      screenActive();
    } else {
        if(ldrValueArr[i] > thresholdLDRValue && !isFreeParking[i]) {  // Car is leaving
          mode = freeParking;
          changeParkingMode(i);
          
          double priceToPay = calcPrice(i);
          screenPay(priceToPay, i);
        }
      }
    }

  distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(distance);
  
  if(distance <= thresholdGPValue && potentialCarWaitAtEntry && anyParking()) { // Car at entry
    potentialCarWaitAtEntry = false;
    openGate();
  }
}

double getVoltage() {
  return analogRead(gpPin) * 5.0 / 1024;
}

double getDistance() {
  return 13 * pow(getVoltage(),-1);           // cm value representation
}

void changeParkingMode(int index) {
  switch(mode) {
    case off: {
      isFreeParking[index] = true;
      digitalWrite(ledPin[index][0], 0);      // Red
      digitalWrite(ledPin[index][1], 0);      // Green

      break;
    }

    case occupiedParking: {
      isFreeParking[index] = false;
      digitalWrite(ledPin[index][0], 255);    // Red
      digitalWrite(ledPin[index][1], 0);      // Green
      timer[index] = rtc.now();
      DateTime entryTime = rtc.now();         

      break;
    }

    case freeParking: {
      isFreeParking[index] = true;
      digitalWrite(ledPin[index][0], 0);      // Red
      digitalWrite(ledPin[index][1], 255);    // Green

      break;
    }
  }  
}

double calcPrice(int i) {
  uint32_t durationInSeconds = calcTime(i);
  double price = (durationInSeconds / 3600.0) * pricePerHour;

  Serial.print("price ");
  Serial.println(price);
  
  return price;
}

uint32_t calcTime(int i) {
  const DateTime entryTime = timer[i];
  
  Serial.print("Entry time: ");
  Serial.print(entryTime.year(), DEC);
  Serial.print('/');
  Serial.print(entryTime.month(), DEC);
  Serial.print('/');
  Serial.print(entryTime.day(), DEC);
  Serial.print(" ");
  Serial.print(entryTime.hour(), DEC);
  Serial.print(':');
  Serial.print(entryTime.minute(), DEC);
  Serial.print(':');
  Serial.println(timer[i].second(), DEC);

  const DateTime exitTime = rtc.now();
  
  Serial.print("Exit time: ");
  Serial.print(exitTime.year(), DEC);
  Serial.print('/');
  Serial.print(exitTime.month(), DEC);
  Serial.print('/');
  Serial.print(exitTime.day(), DEC);
  Serial.print(" ");
  Serial.print(exitTime.hour(), DEC);
  Serial.print(':');
  Serial.print(exitTime.minute(), DEC);
  Serial.print(':');
  Serial.println(exitTime.second(), DEC);

  uint32_t durationInSeconds = exitTime.unixtime() - entryTime.unixtime();
  
  Serial.print("durationInSeconds: ");
  Serial.println(durationInSeconds);

  return durationInSeconds;
}

void moveServoTenderly(int dest, int rate) {
  int current_pos = servo.read();

  if(dest > current_pos) {
    for(int i = current_pos; i <= dest; ++i) {
      servo.write(i);
      
      delay(rate);
    }
  }
  else{
    for(int i = current_pos; i >= dest; --i){
      servo.write(i);
      
      delay(rate);
    }
  }
}

void openGate() {
  moveServoTenderly(degree + 90, 50);             // Opens gate
  
  unsigned long ms_prev_read2 = millis();         // Waits one secound at least for car to pass
  
  while (millis() - ms_prev_read2 < interval);
  while(getDistance() < thresholdGPValue); 
  
  moveServoTenderly(degree, 50);                  // Closes gate
  
  potentialCarWaitAtEntry = true;
}

void screenMain() {
  LcdTouch.fillScreen(MAGENTA);

  LcdTouch.printheb(60,40, "חניון חכם", 3, BLACK);
  LcdTouch.printheb(25,70, " ברוכים הבאים לחניון החכם שמקל על החיים שלכם", 1, BLACK);
  LcdTouch.printheb(0,90,"במערכת תמצאו מידע אודות כמות החניות הפנויות ועוד...", 1, BLACK); 
  
  LcdTouch.drawButton(openSmartParkingLotBtn, 20,  130, 280, 40, 10, BLACK, WHITE, "Open Smart parking lot", 2); 
  
  LcdTouch.setTextSize(1);
  
  while(!LcdTouch.touched()) {
    DateTime now = rtc.now();
    
    LcdTouch.setCursor(65,190);

    if (now.day() < 10) {
      LcdTouch.print("0");
    }
      
    LcdTouch.print(now.day(),DEC);
    LcdTouch.print("/");

    if (now.month() < 10) {
      LcdTouch.print("0");
    }

    LcdTouch.print(now.month(), DEC);
    LcdTouch.print("/");
    
    if (now.year() < 10) {
      LcdTouch.print("0");
    }

    LcdTouch.print(now.year(), DEC);
    LcdTouch.print(" ");
    LcdTouch.print(daysOfTheWeek[now.dayOfTheWeek()]);

    LcdTouch.print(" ");

    if (now.hour() < 10) {
      LcdTouch.print("0");
    }

    LcdTouch.print(now.hour(), DEC);
    LcdTouch.print(":");
    
    if (now.minute() < 10) {
      LcdTouch.print("0");
    }

    LcdTouch.print(now.minute(), DEC);
    LcdTouch.print(":");

    if (now.second() < 10) {
      LcdTouch.print("0");
    }

    LcdTouch.print(now.second(), DEC);
  } 
}

void openParkingLot() { 
  mode = freeParking;

  for(int i = 0; i < PARKINGS; ++i) {
    changeParkingMode(i);
  }
}

void closeParkingLot() {
  mode = off;
  
  for(int i = 0; i < PARKINGS; ++i) {
    changeParkingMode(i);
  }
}

bool anyParking() {
  for(int i = 0; i < PARKINGS; ++i) {
    if(isFreeParking[i]) {
      return true;
    }
  }

  return false;
}

bool allFree() {
  for(int i = 0; i < PARKINGS; ++i){
    if(!isFreeParking[i]) {
      return false;
    }
  }

  return true;
}

void screenActive() {
  LcdTouch.fillScreen(MAGENTA);

  bool anyP = anyParking();
  
  if(anyP) {
    LcdTouch.printheb(20, 10, "מצב: ישנן חניות פנויות", 2, BLACK);
  }  else {
    LcdTouch.printheb(70, 10, "מצב: תפוסה מלאה", 2, BLACK);
  }
  
  if(isFreeParking[1]) {
    LcdTouch.fillRoundRect(170, 60, 100, 100, 10, GREEN);
    LcdTouch.printheb(180, 90, "2", 5, BLACK);
    LcdTouch.printheb(200, 65, "פנוי", 1.5, BLACK);
  }  else {
    LcdTouch.fillRoundRect(170, 60, 100, 100, 10, RED); 
    LcdTouch.printheb(180, 90, "2", 5, BLACK);
    LcdTouch.printheb(200, 65, "תפוס", 1.5, BLACK);
    printTime(190, 150, timer[1],1);
  }

  if(isFreeParking[0]) {
    LcdTouch.fillRoundRect(50, 60, 100, 100, 10, GREEN);
    LcdTouch.printheb(55, 90, "1", 5, BLACK);
    LcdTouch.printheb(80, 65, "פנוי", 1.5, BLACK);
  } else{
    LcdTouch.fillRoundRect(50, 60, 100, 100, 10, RED); 
    LcdTouch.printheb(55, 90, "1", 5, BLACK);
    LcdTouch.printheb(80, 65, "תפוס", 1.5, BLACK);
    printTime(70, 150, timer[0],1);
  }

  if(allFree()) {
    LcdTouch.drawButton(closeBtn, 20,  180, 120, 50, 10, BLACK, WHITE, "close system", 1);
  }
}

void printTime(int x, int y, DateTime time, int textSize) {
  LcdTouch.setCursor(x, y);
  LcdTouch.setTextSize(textSize);
  
  if (time.hour() < 10) {
    LcdTouch.print("0");
  }

  LcdTouch.print(time.hour(), DEC);
  LcdTouch.print(":");

  if (time.minute() < 10) {
    LcdTouch.print("0");
  }

  LcdTouch.print(time.minute(), DEC);
  LcdTouch.print(":");
  
  if (time.second() < 10) {
    LcdTouch.print("0");
  }

  LcdTouch.print(time.second(), DEC);
}

void screenPay(double price, int pIndex) {
  LcdTouch.fillScreen (MAGENTA);
  LcdTouch.printheb(40,40, "סכום לתשלום", 3, BLACK);
  
  String priceStr = String(price, 2);
  LcdTouch.print(60,80, priceStr.c_str(), 2, BLACK);
  LcdTouch.printheb(40,90, "שח", 1, BLACK);
  
  totalProfit += price;
  
  LcdTouch.printheb(200,80, "חניה:", 2, BLACK);
  
  String s = String(pIndex+1);
  LcdTouch.print(188,80,s.c_str(), 2, BLACK);
  
  LcdTouch.printheb(75,120, "כניסה", 1, BLACK);
  printTime(50, 133, timer[pIndex],2);

  LcdTouch.printheb(215,120, "יציאה",1, BLACK);
  printTime(185, 133, rtc.now(),2);

  LcdTouch.drawButton(payBtn, 100,  170, 100, 50, 10, BLACK, WHITE, "Paid", 2); 

  while (!LcdTouch.touched());
}

void screenClose() { // Only if the parking lot is empty
  closeParkingLot();
  
  LcdTouch.fillScreen (MAGENTA);
  LcdTouch.printheb(60,40, "החניון סגור", 3, BLACK);
  
  LcdTouch.printheb(120,80, "רווח כללי להיום", 1.5, BLACK);
  
  String totalProfitStr = String(totalProfit, 4);
  LcdTouch.print(140,100, totalProfitStr.c_str(), 2, BLACK);
  LcdTouch.printheb(110,105, "שח", 1, BLACK);
  
  LcdTouch.drawButton(openSmartParkingLotBtn, 110,  180, 100, 50, 10, BLACK, WHITE, "Restart", 2); 
  
  while (!LcdTouch.touched());
}

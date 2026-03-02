//Title: Thermomitor and Humidity sensor/alarm
//Author: Jeremiah Wenzel


#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "pitches.h"
#include <DHT.h>
#include "LedControl.h"
#include <TouchScreen.h>
#include <Wire.h>
#include <DS3231.h>
#include <EEPROM.h>

DS3231 clock;
RTCDateTime dt;

#define MINPRESSURE 10
#define MAXPRESSURE 1000
//Touch For New ILI9341 TP
#define TS_LEFT 118
#define TS_RT 925

#define TS_TOP 70
#define TS_BOT 910

#define DHT_SENSOR_TYPE DHT_TYPE_22
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
//setup pins
#define BLUEL  46
#define GREENL 45
#define REDL  44
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
static const int DHT_SENSOR_PIN = 49;
DHT dht22(DHT_SENSOR_PIN, DHT22);

//make color selection easy
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
//initialize tft
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define USE_Elegoo_SHIELD_PINOUT
//Elegoo_TFTLCD tft;
    int rotated_x, rotated_y;

#define TS_MINX 118 // (LEFT)
#define TS_MAXX 925 // (RIGHT)
#define TS_MINY  70 // (TOP)
#define TS_MAXY 910 // (BOTTOM)
//variables for the project
int16_t px = 0;
int16_t py = 0;
float temperature;
float humidity;
float tempF;
int hmatrix;
int hours;
int minutes;
//DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
// establish temp ranges
float low_value = 40;
float gap_value = 20;
float high_value = 65;

// EEPROM addresses
#define EEPROM_LOW_ADDR  0
#define EEPROM_GAP_ADDR  4
#define EEPROM_HIGH_ADDR 8
#define EEPROM_MAGIC_ADDR 12
#define EEPROM_MAGIC_VALUE 0xAB
int count;
//speaker settings
int counter = 1000;
int redValue;
int greenValue;
int blueValue;
//menuvariables
  Elegoo_GFX_Button buttons[7]; //0,210,75,30
int duration = 500;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
unsigned long delaytime1=500;
unsigned long delaytime2=50;
int dht_timer = 2000;
int beep_timer = 2000;
int melody_timer = 500;
unsigned long lastExecutedMillis_1 = 0;
unsigned long lastExecutedMillis_2 = 0;
unsigned long lastExecutedMillis_3 = 0;
int thisNote = 0;
// --- EEPROM helpers ---
void saveTempsToEEPROM() {
  EEPROM.put(EEPROM_LOW_ADDR, low_value);
  EEPROM.put(EEPROM_GAP_ADDR, gap_value);
  EEPROM.put(EEPROM_HIGH_ADDR, high_value);
}

void loadTempsFromEEPROM() {
  if (EEPROM.read(EEPROM_MAGIC_ADDR) == EEPROM_MAGIC_VALUE) {
    EEPROM.get(EEPROM_LOW_ADDR, low_value);
    EEPROM.get(EEPROM_GAP_ADDR, gap_value);
    EEPROM.get(EEPROM_HIGH_ADDR, high_value);
  } else {
    // First boot — write defaults
    low_value = 40;
    gap_value = 20;
    high_value = 65;
    saveTempsToEEPROM();
    EEPROM.write(EEPROM_MAGIC_ADDR, EEPROM_MAGIC_VALUE);
  }
}

// --- Forward declarations ---
void redrawMainScreen();
void enterDateScreen();

// --- Helper: draw a temp value with dynamic font size ---
// Uses size 3 for 1-2 digit numbers, size 2 for 3+ digits
void drawTempValue(int rectX, int cursorX, int y, float val) {
  int intVal = (int)floor(val);
  int absVal = abs(intVal);
  bool threeDigit = (absVal >= 100 || intVal <= -10);

  tft.fillRect(rectX, y, 40, 30, BLACK);
  tft.setTextColor(GREEN);
  if (threeDigit) {
    tft.setTextSize(2);
  } else {
    tft.setTextSize(3);
  }
  tft.setCursor(cursorX, y + 5);
  tft.print(floor(val), 0);
}

// --- Helper: days in a given month/year ---
int daysInMonth(int month, int year) {
  switch (month) {
    case 2:
      if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) return 29;
      return 28;
    case 4: case 6: case 9: case 11:
      return 30;
    default:
      return 31;
  }
}

// --- Date/Time Setting Screen ---
void enterDateScreen() {
  dt = clock.getDateTime();
  int setMonth = dt.month;
  int setDay = dt.day;
  int setYear = dt.year;
  int setHour = dt.hour;
  int setMinute = dt.minute;

  tft.fillScreen(BLACK);

  // Draw Cancel and Save buttons
  Elegoo_GFX_Button btnCancel, btnSave;
  btnCancel.initButton(&tft, 50, 20, 80, 30, RED, RED, WHITE, "Cancel", 2);
  btnCancel.drawButton();
  btnSave.initButton(&tft, 270, 20, 80, 30, GREEN, GREEN, BLACK, "Save", 2);
  btnSave.drawButton();

  // Layout: 5 columns for Month, Day, Year, Hour, Minute
  // X positions for each column
  int colX[5] = {25, 85, 160, 230, 285};
  int valY = 120;  // Y for value display
  int plusY = 85;   // Y for + buttons
  int minusY = 160; // Y for - buttons

  // Draw labels
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(5, 50);
  tft.print("Mon Day  Year  Hr  Min");

  // + and - buttons for each field
  Elegoo_GFX_Button plusBtns[5], minusBtns[5];
  for (int i = 0; i < 5; i++) {
    int bw = (i == 2) ? 50 : 35; // wider for year
    plusBtns[i].initButton(&tft, colX[i], plusY, bw, 25, BLUE, BLUE, WHITE, "+", 2);
    plusBtns[i].drawButton();
    minusBtns[i].initButton(&tft, colX[i], minusY, bw, 25, BLUE, BLUE, WHITE, "-", 2);
    minusBtns[i].drawButton();
  }

  // Helper lambda-like: draw all values
  auto drawValues = [&]() {
    tft.setTextSize(2);
    tft.setTextColor(GREEN, BLACK);
    // Month
    tft.setCursor(colX[0] - 10, valY);
    if (setMonth < 10) tft.print("0");
    tft.print(setMonth);
    tft.print(" ");
    // Day
    tft.setCursor(colX[1] - 10, valY);
    if (setDay < 10) tft.print("0");
    tft.print(setDay);
    tft.print(" ");
    // Year
    tft.setCursor(colX[2] - 20, valY);
    tft.print(setYear);
    tft.print(" ");
    // Hour
    tft.setCursor(colX[3] - 10, valY);
    if (setHour < 10) tft.print("0");
    tft.print(setHour);
    tft.print(" ");
    // Minute
    tft.setCursor(colX[4] - 10, valY);
    if (setMinute < 10) tft.print("0");
    tft.print(setMinute);
    tft.print(" ");
  };

  drawValues();

  // Draw separator hints
  tft.setTextColor(WHITE);
  tft.setCursor(colX[1] + 18, valY);
  tft.print("/");
  tft.setCursor(colX[2] + 28, valY);
  tft.print(" ");
  tft.setCursor(colX[4] - 30, valY);
  tft.print(":");

  // Blocking touch loop for date screen
  while (true) {
    digitalWrite(13, HIGH);
    TSPoint p = ts.getPoint();
    digitalWrite(13, LOW);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    int tx = 0, ty = 0;
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      tx = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
      ty = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
    } else {
      // No touch — update button states to released
      btnCancel.press(false);
      btnSave.press(false);
      for (int i = 0; i < 5; i++) {
        plusBtns[i].press(false);
        minusBtns[i].press(false);
      }
      // Redraw released buttons
      if (btnCancel.justReleased()) btnCancel.drawButton();
      if (btnSave.justReleased()) btnSave.drawButton();
      for (int i = 0; i < 5; i++) {
        if (plusBtns[i].justReleased()) plusBtns[i].drawButton();
        if (minusBtns[i].justReleased()) minusBtns[i].drawButton();
      }
      continue;
    }

    // Check Cancel/Save
    btnCancel.press(btnCancel.contains(tx, ty));
    btnSave.press(btnSave.contains(tx, ty));

    // Check +/- buttons
    for (int i = 0; i < 5; i++) {
      plusBtns[i].press(plusBtns[i].contains(tx, ty));
      minusBtns[i].press(minusBtns[i].contains(tx, ty));
    }

    // Handle Cancel
    if (btnCancel.justPressed()) {
      btnCancel.drawButton(true);
      delay(200);
      redrawMainScreen();
      return;
    }

    // Handle Save
    if (btnSave.justPressed()) {
      btnSave.drawButton(true);
      delay(200);
      clock.setDateTime(setYear, setMonth, setDay, setHour, setMinute, 0);
      redrawMainScreen();
      return;
    }

    bool changed = false;

    // Handle + buttons
    for (int i = 0; i < 5; i++) {
      if (plusBtns[i].justPressed()) {
        plusBtns[i].drawButton(true);
        switch (i) {
          case 0: setMonth++; if (setMonth > 12) setMonth = 1; break;
          case 1: setDay++; if (setDay > daysInMonth(setMonth, setYear)) setDay = 1; break;
          case 2: setYear++; break;
          case 3: setHour++; if (setHour > 23) setHour = 0; break;
          case 4: setMinute++; if (setMinute > 59) setMinute = 0; break;
        }
        changed = true;
        delay(200);
      }
    }

    // Handle - buttons
    for (int i = 0; i < 5; i++) {
      if (minusBtns[i].justPressed()) {
        minusBtns[i].drawButton(true);
        switch (i) {
          case 0: setMonth--; if (setMonth < 1) setMonth = 12; break;
          case 1: setDay--; if (setDay < 1) setDay = daysInMonth(setMonth, setYear); break;
          case 2: setYear--; break;
          case 3: setHour--; if (setHour < 0) setHour = 23; break;
          case 4: setMinute--; if (setMinute < 0) setMinute = 59; break;
        }
        changed = true;
        delay(200);
      }
    }

    if (changed) {
      // Clamp day if month changed
      int maxDay = daysInMonth(setMonth, setYear);
      if (setDay > maxDay) setDay = maxDay;
      drawValues();
    }

    // Redraw released buttons
    if (btnCancel.justReleased()) btnCancel.drawButton();
    if (btnSave.justReleased()) btnSave.drawButton();
    for (int i = 0; i < 5; i++) {
      if (plusBtns[i].justReleased()) plusBtns[i].drawButton();
      if (minusBtns[i].justReleased()) minusBtns[i].drawButton();
    }
  }
}

// --- Redraw main screen after returning from date screen ---
void redrawMainScreen() {
  tft.fillScreen(BLACK);
  tft.setTextSize(3);
  tft.setTextColor(GREEN);

  // Redraw temp +/- buttons
  buttons[0].initButton(&tft, 20, 220, 20, 20, BLUE, BLUE, WHITE, "+", 3);
  buttons[0].drawButton();
  buttons[1].initButton(&tft, 100, 220, 20, 20, BLUE, BLUE, WHITE, "-", 3);
  buttons[1].drawButton();
  buttons[2].initButton(&tft, 130, 220, 20, 20, BLUE, BLUE, WHITE, "+", 3);
  buttons[2].drawButton();
  buttons[3].initButton(&tft, 200, 220, 20, 20, BLUE, BLUE, WHITE, "-", 3);
  buttons[3].drawButton();
  buttons[4].initButton(&tft, 225, 220, 20, 20, BLUE, BLUE, WHITE, "+", 3);
  buttons[4].drawButton();
  buttons[5].initButton(&tft, 295, 220, 20, 20, BLUE, BLUE, WHITE, "-", 3);
  buttons[5].drawButton();

  // Set Date button
  buttons[6].initButton(&tft, 280, 15, 70, 25, CYAN, CYAN, BLACK, "Date", 2);
  buttons[6].drawButton();

  // Temp value displays
  drawTempValue(40, 41, 205, low_value);
  drawTempValue(145, 146, 205, gap_value);
  drawTempValue(240, 241, 205, high_value);

  // Labels
  tft.setTextSize(3);
  tft.setTextColor(GREEN);
  tft.setCursor(20, 180);
  tft.print("Low");
  tft.setCursor(140, 180);
  tft.print("Gap");
  tft.setCursor(230, 180);
  tft.print("High");

  // Reset touch coordinates
  px = 0;
  py = 0;
}

void setup()
{
    Serial.begin(9600);
  //setup clock
 clock.begin();
 
 //bool h12 = false;
 //void setClockMode(bool h12);
//clock.setDateTime(2025, 7, 2, 22, 44, 10);
 // clock.setDateTime(__DATE__, __TIME__);
  //setup rgb led
  pinMode(REDL, OUTPUT);
  pinMode(GREENL, OUTPUT);
  pinMode(BLUEL, OUTPUT);
  //Write(RED, 255);
  
  digitalWrite(REDL, HIGH);
  digitalWrite(GREENL, LOW);
  digitalWrite(BLUEL, LOW);
  //setup tft
   
  tft.setTextColor(GREEN);  
    tft.setTextSize(3);
    Serial.begin(9600);
  Serial.println(F("TFT LCD test")); 
  tft.fillScreen(BLACK);
  tft.setCursor(0,0);

  
//setup settings button
#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  //tft.reset();

   uint16_t identifier = 0x9341;
   
  tft.begin(identifier);
  tft.setRotation(3);

  // Load saved temp thresholds from EEPROM
  loadTempsFromEEPROM();

  buttons[0].initButton(&tft,20,220,20,20,BLUE,BLUE,WHITE,"+",3);
  buttons[0].drawButton();
   buttons[1].initButton(&tft,100,220,20,20,BLUE,BLUE,WHITE,"-",3);
  buttons[1].drawButton();
   buttons[2].initButton(&tft,130,220,20,20,BLUE,BLUE,WHITE,"+",3);
  buttons[2].drawButton();
   buttons[3].initButton(&tft,200,220,20,20,BLUE,BLUE,WHITE,"-",3);
  buttons[3].drawButton();
   buttons[4].initButton(&tft,225,220,20,20,BLUE,BLUE,WHITE,"+",3);
  buttons[4].drawButton();
   buttons[5].initButton(&tft,295,220,20,20,BLUE,BLUE,WHITE,"-",3);
  buttons[5].drawButton();
  // Set Date button (top right)
  buttons[6].initButton(&tft,280,15,70,25,CYAN,CYAN,BLACK,"Date",2);
  buttons[6].drawButton();
      //setup buttons and labels
      drawTempValue(40, 41, 205, low_value);
      drawTempValue(145, 146, 205, gap_value);
      drawTempValue(240, 241, 205, high_value);
    tft.setTextSize(3);
    tft.setTextColor(GREEN);
    tft.setCursor(20,180);
    tft.print("Low");
    tft.setCursor(140,180);
    tft.print("Gap");
    tft.setCursor(230,180);
    tft.print("High");
  //initialize dht
  dht22.begin();
}



void loop()
{
dt = clock.getDateTime();
  //timers
  unsigned long currentMillis = millis();
 //test code
 digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
 pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
   {
  // Map the touch coordinates to the new orientation
    px = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
        py = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
   }

  
 for (uint8_t b=0; b<7; b++)
  {
    if (buttons[b].contains(px, py))
    {
      Serial.print("Pressing: "); Serial.println(b);
      buttons[b].press(true);
    } else
    {
      buttons[b].press(false);
    }
  }

for (uint8_t b=0; b<7; b++) {
    if (buttons[b].justReleased()) {
       Serial.print("Released: "); Serial.println(b);
      buttons[b].drawButton();  // draw normal
    }

    if (buttons[b].justPressed()) {
        buttons[b].drawButton(true);  // draw invert!

        switch (b) {
          case 0: // Low +
            low_value += 1;
            drawTempValue(40, 41, 205, low_value);
            saveTempsToEEPROM();
            break;
          case 1: // Low -
            low_value -= 1;
            drawTempValue(40, 41, 205, low_value);
            saveTempsToEEPROM();
            break;
          case 2: // Gap +
            gap_value += 1;
            drawTempValue(145, 146, 205, gap_value);
            saveTempsToEEPROM();
            break;
          case 3: // Gap -
            gap_value -= 1;
            drawTempValue(145, 146, 205, gap_value);
            saveTempsToEEPROM();
            break;
          case 4: // High +
            high_value += 1;
            drawTempValue(240, 241, 205, high_value);
            saveTempsToEEPROM();
            break;
          case 5: // High -
            high_value -= 1;
            drawTempValue(240, 241, 205, high_value);
            saveTempsToEEPROM();
            break;
          case 6: // Set Date
            enterDateScreen();
            return; // Exit loop() iteration since screen was redrawn
        }

      delay(100); // UI debouncing
      // Reset touch to prevent repeated triggers
      px = 0;
      py = 0;
    }
  }

  tft.setCursor(0,0);
 // print out a sucessfull measurement on lcd 

  //read temp and humidity
 tempF = dht22.readTemperature(true);
 humidity = dht22.readHumidity();
 // send humidity data to dot matrix lcd
 //tempF = temperature * 9.0 / 5.0 + 32.0;
 tft.setTextSize(3);
 tft.setTextColor(GREEN);

  // tft.drawRect(rectx,recty,140,25, BLACK);
  if (currentMillis - lastExecutedMillis_1 >= dht_timer)
  {
    lastExecutedMillis_1 = currentMillis;
   tft.println("Temperature:");
  tft.fillRect(0,29,120,25, BLACK);
  
   tft.setCursor(0,30);
    tft.print(tempF);
    tft.println("F");
   tft.setCursor(0,55); 
    tft.println("Humidity:");
  
  tft.fillRect(0,76,120,25, BLACK);
    tft.print(humidity);
    tft.println("%");
    tft.setCursor(0,106);
    tft.println("Date/Time");
    tft.fillRect(0,130,320,25, BLACK);
    //tft.print(clock.dateFormat("jS M y, h:ia", dt));
    tft.setCursor(0,132);
     if (dt.month <10)
    {
      tft.print("0");
    }
    tft.print(dt.month);
    tft.print("/");
     if (dt.day <10)
    {
      tft.print("0");
    }
    tft.print(dt.day);
    tft.print("/");
    tft.print(dt.year);
    tft.print(" ");
    hours = dt.hour;
    if (hours > 12)
    {
      hours = hours - 12;
      if (hours < 10)
      {
        tft.print("0");
      }
      tft.print(hours);
    }
    else 
    {
        if (hours != 0 && hours < 10)
      {
        tft.print("0");
      }
      if (hours == 0)
      {
        tft.print("12");
      }
      else
      {
      tft.print(dt.hour);
      }
    }
    tft.print(":");
    if (dt.minute <10)
    {
      tft.print("0");
    }
    tft.print(dt.minute);
    if (dt.hour > 12)
    {
      tft.print("P");
    }
    else
    {
      tft.print("A");
    }
  }
  
 
 //change LED and or beep depending on temperature ranges
  if (tempF >= high_value)
  {
    // At or above high — RED
    redValue = 255;
    greenValue = 0;
    blueValue = 0;
  }
  else if (tempF >= (high_value - gap_value))
  {
    // Within gap below high — YELLOW
    redValue = 255;
    greenValue = 255;
    blueValue = 0;
  }
  else if (tempF <= low_value)
  {
    // At or below low — BLUE
    redValue = 0;
    greenValue = 0;
    blueValue = 255;
  }
  else if (tempF <= (low_value + gap_value))
  {
    // Within gap above low — YELLOW
    redValue = 255;
    greenValue = 255;
    blueValue = 0;
  }
  else
  {
    // Outside both gap ranges — GREEN
    redValue = 0;
    greenValue = 255;
    blueValue = 0;
  }
  analogWrite(REDL, redValue);
  analogWrite(GREENL, greenValue);
  analogWrite(BLUEL, blueValue);   
     
}
 
 

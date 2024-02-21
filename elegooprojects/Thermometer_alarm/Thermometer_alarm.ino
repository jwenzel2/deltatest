//Title: Thermomitor and Humidity sensor/alarm
//Author: Jeremiah Wenzel


#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "pitches.h"
#include <DHT.h>
#include "LedControl.h"
#include <TouchScreen.h>


#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920
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
int speaker = 50;
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
int rectx = 0;
int recty= 20;

//variables for the project
float temperature;
float humidity;
float tempF;
int hmatrix;
//DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
// establish temp ranges
float coldtemp = 40;
float lowtemp = 50;
float hightemp = 65;
bool beep_enabled = true;
int count;
//speaker settings
int counter = 1000;
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int redValue;
int greenValue;
int blueValue;
//menuvariables
unsigned long menuTriggeredTime = 0;
bool menu_active = false;
const int numOfScreens = 4;
int parameters[numOfScreens] = {coldtemp,lowtemp, hightemp,0};
String screens[numOfScreens] = {"coldtemp","Low Temp","High Temp","Exiting..."};
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
void setup()
{
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
      //setup buttons and labels
      tft.setTextColor(GREEN);
 tft.fillRect(40,205,40,30, BLACK);
    tft.setCursor(41,210);
    tft.print(floor(lowtemp),0);
    tft.fillRect(145,205,40,30, BLACK);
    tft.setCursor(146,210);
    tft.print(floor(lowtemp),0);
      tft.fillRect(240,205,40,30, BLACK);
    tft.setCursor(241,210);
    tft.print(floor(hightemp),0);
    tft.setCursor(20,180);
    tft.print("Cold");
    tft.setCursor(140,180);
    tft.print("Low");
    tft.setCursor(230,180);
    tft.print("High");
  //initialize dht
  dht22.begin();
}



void loop()
{

  //timers
  unsigned long currentMillis = millis();
//test code
 digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
 pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.y, TS_MINX, TS_MAXX, tft.height(), 0);
    p.y = (tft.width()-map(p.x, TS_MINY, TS_MAXY, tft.width(), 0));
   }
    if (buttons[0].contains(p.y, p.x)) {
      //Serial.print("Pressing: "); Serial.println(b);
      buttons[0].press(true); 
     // tell the button it is pressed
    } else {
      buttons[0].press(false);  // tell the button it is NOT pressed
    }
  if (buttons[0].justPressed()) {
        buttons[0].drawButton(true); 
        if (beep_enabled = true)
        {
          beep_enabled = false;
        }
        else 
        {
          beep_enabled == true;
        } // draw invert!
  }
   if (buttons[0].justReleased()) {
      // Serial.print("Released: "); Serial.println(b);
      buttons[0].drawButton();  // draw normal
    }
 
  tft.setCursor(0,0);
// print out a sucessfull measurement on lcd 

  //read temp and humidity
 tempF = dht22.readTemperature();
 humidity = dht22.readHumidity();
 // send humidity data to dot matrix lcd
 //tempF = temperature * 9.0 / 5.0 + 32.0;
 tft.setTextColor(GREEN);
  
  // tft.drawRect(rectx,recty,140,25, BLACK);
  if (currentMillis - lastExecutedMillis_1 >= dht_timer)
  {
    lastExecutedMillis_1 = currentMillis;
   tft.println("Temperature:");
    tft.fillRect(0,72,110,25, BLACK);
  tft.fillRect(rectx,recty,120,26, BLACK);
    //tft.print(tempF);
   
    tft.print(tempF);
    tft.println("C");
    tft.println("Humidity:");
  //   tft.drawRect(rectx,recty,140,50, BLACK);

    tft.print(humidity);
    tft.println("%");


  }
  
 
//change LED and or beep depending on temperature ranges
  if (tempF <= lowtemp && tempF >= coldtemp)
  {
    greenValue = 255;
    redValue = 0;
    blueValue= 0;
    analogWrite(REDL, redValue);
    analogWrite(GREENL, greenValue);
    analogWrite(BLUEL, blueValue);
    
  }
  if (tempF <= hightemp && tempF >= lowtemp)
  {
    redValue = 255;
    greenValue= 255;
    blueValue= 0;
    analogWrite(REDL, redValue);
    analogWrite(GREENL, greenValue);
    analogWrite(BLUEL, blueValue);
    
  }
  if (tempF <= coldtemp)
    {
      redValue = 0;
      blueValue = 255;
      greenValue = 0;
      analogWrite(REDL, redValue);
      analogWrite(GREENL, greenValue);
      analogWrite(BLUEL, blueValue);
      if (beep_enabled == true)
      {   
        if (currentMillis >= beep_timer)
        {   
        for (int thisNote = 0; thisNote < 8; thisNote++) 
        {
          // pin8 output the voice, every scale is 0.5 sencond
          tone(3, melody[thisNote], duration);
          delay(500);
          // Output the voice after several minutes
          
        }
    
        }
      }
    }
      if (tempF > hightemp)
        { 
          redValue = 255;
          blueValue = 0;
          greenValue = 0;
          analogWrite(REDL, redValue);
          analogWrite(GREENL, greenValue);
          analogWrite(BLUEL, blueValue);
          if (beep_enabled == true)
          {
            if (currentMillis - lastExecutedMillis_2 >= beep_timer)
            {
              lastExecutedMillis_2 >= currentMillis;
              // pin8 output the voice, every scale is 0.5 sencond
           
           
            if (currentMillis - lastExecutedMillis_3 >= melody_timer)
            {
               lastExecutedMillis_3 = currentMillis;
             tone(speaker, melody[thisNote], duration);
            
             if (thisNote <= 8)
             {
              thisNote++;
             }
             else if(thisNote = 8)
             {
              thisNote = 0;
             }
            // Output the voice after several minutes
            }
            
    
          }
  
    
        }   
     }
   }
 
//} 


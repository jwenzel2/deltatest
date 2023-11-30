//Title: Thermomitor and Humidity sensor/alarm
//Author: Jeremiah Wenzel


#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "pitches.h"
#include <DHT.h>
#include "LedControl.h"
#include <SPI.h>
#include <TFT.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
//setup pins
#define BLUEL  46
#define GREENL 45
#define REDL  44

static const int DHT_SENSOR_PIN = 49;
DHT dht11(DHT_SENSOR_PIN, DHT11);
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
int rectx = 0;
int recty= 20;
//variables for the project
float temperature;
float humidity;
float tempF;
int hmatrix;
//DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
// establish temp ranges
float coldtemp = 70.0;
float lowtemp = 75.0;
float hightemp = 82.0;
bool beep_enabled = false;
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

int duration = 500;

unsigned long delaytime1=500;
unsigned long delaytime2=50;

void setup()
{
  //setup rgb led
  pinMode(REDL, OUTPUT);
  pinMode(GREENL, OUTPUT);
  pinMode(BLUEL, OUTPUT);
  //analogWrite(RED, 255);
  
  //digitalWrite(REDL, HIGH);
  //digitalWrite(GREENL, LOW);
 // digitalWrite(BLUEL, LOW);
  //setup tft
  
  tft.setTextColor(GREEN);  
    tft.setTextSize(3);
    Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  tft.fillScreen(BLACK);

#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  //tft.reset();

   uint16_t identifier = tft.readID();
   if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }
  else if(identifier==0x1111)
  {     
      identifier=0x9328;
       Serial.println(F("Found 0x9328 LCD driver"));
  }
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9328;
  
  }
  tft.begin(identifier);
  tft.setRotation(3);

  
  
  
  //initialize dht
  dht11.begin();
}

//configuration menu
void printScreen() {
//currentScreen = 0;
delay(500);
//clear tft
//tft.reset();
menu_active = true;

}

void loop()
{
  //work with the menu when it has been activated
  /*needs adaptation to TFT
  while (menu_active == true)
  { 
    if (digitalRead(forwardpin) == LOW)
    {
   
      if (currentScreen < 5)
      {
        currentScreen++;
        delay(500);
      }
    
    }  
    if (digitalRead(backwardpin) == LOW)
    {
      if (currentScreen > 0)
      {
        currentScreen--;
        delay(500);
      }
    } 
    if (digitalRead(positivepin) == LOW)
    {
      int value = parameters[currentScreen];
      value++;
      delay(500);
      if (currentScreen == 0)
      {
        lowtemp = value;
      }
      if (currentScreen == 1)
      {
        hightemp = value;
      }
      parameters[currentScreen] = value;
    }
    if (digitalRead(negativepin) == LOW)
    {
      int value = parameters[currentScreen];
      value--;
      delay(500);
      if (currentScreen == 0)
      {
        lowtemp = value;
      }
      if (currentScreen == 1)
      {
       hightemp = value;
      }
      parameters[currentScreen] = value;
    
    }
      if (currentScreen == 3) 
      {
        menu_active = false;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("RESUMING PROGRAM");
        delay(500);
        lcd.clear();
      }
    else 
      {
        lcd.setCursor(0,0);
        lcd.print(screens[currentScreen]);
        lcd.print(":      ");
        lcd.setCursor(0,1);
        lcd.print(parameters[currentScreen]);
      }
  }
*/
 //while (menu_active == false)
 //{

 //tft.fillScreen(BLACK);
  tft.setCursor(0,0);
// print out a sucessfull measurement on lcd 
  delay(1000);
  //read temp and humidity
 tempF = dht11.readTemperature(true);
 humidity = dht11.readHumidity();
 // send humidity data to dot matrix lcd
 //tempF = temperature * 9.0 / 5.0 + 32.0;
 
    tft.println("Temperature:");
   // tft.drawRect(rectx,recty,140,25, BLACK);
    tft.fillRect(rectx,recty,140,25, BLACK);
    tft.print(tempF);
    tft.println("F");
    tft.println("Humidity:");
  //   tft.drawRect(rectx,recty,140,50, BLACK);
    tft.fillRect(0,73,140,25, BLACK);
    tft.print(humidity, 1);
    tft.println("%");


  tft.setCursor(0,1);
  delay(1000);
  tft.print("                ");
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
      tft.setCursor(0,1);
      tft.print("Too Cold!        ");
      redValue = 0;
      blueValue = 255;
      greenValue = 0;
      analogWrite(REDL, redValue);
      analogWrite(GREENL, greenValue);
      analogWrite(BLUEL, blueValue);
      if (beep_enabled == true)
      {      
        for (int thisNote = 0; thisNote < 8; thisNote++) 
        {
          // pin8 output the voice, every scale is 0.5 sencond
          tone(3, melody[thisNote], duration);
     
          // Output the voice after several minutes
          delay(500);
        }
    
        delay(2000);
      }
      if (tempF > hightemp)
        { 
     
          tft.setCursor(0,1);
          tft.print("Temp Alarm hit        ");
          redValue = 255;
          blueValue = 0;
          greenValue = 0;
          analogWrite(REDL, redValue);
          analogWrite(GREENL, greenValue);
          analogWrite(BLUEL, blueValue);
          if (beep_enabled == true)
          {
     
            for (int thisNote = 0; thisNote < 8; thisNote++) 
            {
            // pin8 output the voice, every scale is 0.5 sencond
            tone(3, melody[thisNote], duration);
     
            // Output the voice after several minutes
            delay(500);
            }
    
          delay(2000);
  
    
        }   
     }
   }
 }
//} 


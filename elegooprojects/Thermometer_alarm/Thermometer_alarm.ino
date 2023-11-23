//Title: Thermomitor and Humidity sensor/alarm
//Author: Jeremiah Wenzel


#include <LiquidCrystal.h>
#include "pitches.h"
#include <DHT.h>
#include "LedControl.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11

//setup pins
#define BLUE 11
#define GREEN 12
#define RED 13
static const int DHT_SENSOR_PIN = 4;
DHT dht11(DHT_SENSOR_PIN, DHT11);
int speaker = 3;
int menupin = 18;
int forwardpin = 28;
int backwardpin = 30;
int negativepin = 34;
int positivepin = 32;
int DIN = 22;
int CS = 24;
int CLK = 26;
int RS= 10;
int LEN = 9;
int D4 = 8;
int D5 = 7;
int D6 = 6;
int D7 = 5;
LiquidCrystal lcd(RS, LEN, D4, D5, D6, D7);
LedControl lc=LedControl(DIN,CLK,CS,1);
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
int buttonStateMenu = 0; 
int buttonStatePlus = 0;
int buttonStateNegative = 0;
int buttonStateOk = 0;
int currentScreen = 0;
int prevbuttonStateMenu = 0; 
int prevbuttonStatePlus = 0;
int prevbuttonStateNegative = 0;
int prevbuttonStateOk = 0;
const int numOfScreens = 4;
int parameters[numOfScreens] = {coldtemp,lowtemp, hightemp,0};
String screens[numOfScreens] = {"coldtemp","Low Temp","High Temp","Exiting..."};

int duration = 500;

unsigned long delaytime1=500;
unsigned long delaytime2=50;

void setup()
{
  //setup lcd
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("INITIALIZING!");
  //setup dot matrix
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  
  //setup rgb led
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  //setup buttons
  pinMode(menupin, INPUT_PULLUP);
  pinMode(positivepin, INPUT_PULLUP);
  pinMode(negativepin,INPUT_PULLUP);
  pinMode(forwardpin,INPUT_PULLUP);
  pinMode(backwardpin, INPUT_PULLUP);
  prevbuttonStatePlus = digitalRead(positivepin);

  //initialize dht
  dht11.begin();
 

  //setup menu interupt
  attachInterrupt(digitalPinToInterrupt(menupin),printScreen, LOW);
}

//configuration menu
void printScreen() {
currentScreen = 0;
delay(500);
lcd.clear();
menu_active = true;

}
//show humidity level on dot matrix led
void single(int count) {
  //initial everything off
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      delay(delaytime2);
      lc.setLed(0,row,col,false);
      delay(delaytime2);
      for(int i=0;i<col;i++) {
        lc.setLed(0,row,col,false);
        delay(delaytime2);
      }
    }
  }
  //now light enough lights to indicate humidyt percentage
  while(count != 0)
  {
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      delay(delaytime2);
      lc.setLed(0,row,col,true);
      count--;
      delay(delaytime2);
    }
  }
  }
  return ;
}
//test function end
void loop()
{
  //work with the menu when it has been activated
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

 while (menu_active == false)
 {

  if(digitalRead(backwardpin) == LOW && digitalRead(forwardpin) == LOW)
  {
    beep_enabled = !beep_enabled;
  }
  
  lcd.setCursor(0,0);
// print out a sucessfull measurement on lcd 
  delay(1000);
  //read temp and humidity
 tempF = dht11.readTemperature(true);
 humidity = dht11.readHumidity();
 // send humidity data to dot matrix lcd
 hmatrix = humidity;
 single(hmatrix);
 //tempF = temperature * 9.0 / 5.0 + 32.0;
    lcd.print("T:");
    lcd.print(tempF);
    lcd.print("F H:");
    lcd.print(humidity, 1);
    lcd.print("%");


  lcd.setCursor(0,1);
  delay(1000);
  lcd.print("                ");
//change LED and or beep depending on temperature ranges
  if (tempF <= lowtemp && tempF >= coldtemp)
  {
    greenValue = 255;
    redValue = 0;
    blueValue= 0;
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    
  }
  if (tempF <= hightemp && tempF >= lowtemp)
  {
    redValue = 255;
    greenValue= 255;
    blueValue= 0;
     analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    
  }
  if (tempF <= coldtemp)
    {
      lcd.setCursor(0,1);
      lcd.print("Too Cold!        ");
      redValue = 0;
      blueValue = 255;
      greenValue = 0;
      analogWrite(RED, redValue);
      analogWrite(GREEN, greenValue);
      analogWrite(BLUE, blueValue);
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
     
          lcd.setCursor(0,1);
          lcd.print("Temp Alarm hit        ");
          redValue = 255;
          blueValue = 0;
          greenValue = 0;
          analogWrite(RED, redValue);
          analogWrite(GREEN, greenValue);
          analogWrite(BLUE, blueValue);
          // pin8 output the voice, every scale is 0.5 sencond
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
} 


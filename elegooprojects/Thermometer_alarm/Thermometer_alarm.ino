//www.elegoo.com
//2016.12.9

#include <LiquidCrystal.h>
#include "pitches.h"
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11
//setup pins
#define BLUE 5
#define GREEN 4
#define RED 2
static const int DHT_SENSOR_PIN = 13;
int speaker = 3;
int menupin = 18;
int forwardpin = 28;
int backwardpin = 30;
int negativepin = 34;
int positivepin = 32;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
//variables for the project
float temperature;
float humidity;
float tempF;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
// establish temp ranges
float lowtemp = 75.0;
float hightemp = 82.0;
bool beep_enabled = false;
//speaker settings
int counter = 500;
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
const int numOfScreens = 3;
int parameters[numOfScreens] = {lowtemp, hightemp,0};
String screens[numOfScreens] = {"Low Temp","High Temp","Exiting..."};




void setup()
{
  //setup lcd
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("INITIALIZING!");
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
  //setup menu interupt
  attachInterrupt(digitalPinToInterrupt(menupin),printScreen, LOW);
}
// take a temperature and humidity measurement
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

//configuration menu in progress
void printScreen() {
currentScreen = 0;
lcd.clear();
menu_active = true;
}

void loop()
{

  while (menu_active == true)
{ 
  if (digitalRead(forwardpin) == LOW)
  {
   
    if (currentScreen < 4)
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
    if (currentScreen == 2) {
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

 if (menu_active == false)
 {

  if (digitalRead(negativepin == LOW))
  {
    if (beep_enabled == false)
      {
        beep_enabled == true;
        delay(500);
      }
      if (beep_enabled == true)
      {
        beep_enabled == false;
        delay(500);
      }
  }
  lcd.setCursor(0,0);
// print out a sucessfull measurement on lcd 
if( measure_environment( &temperature, &humidity ) == true )
  { 
     tempF = temperature * 9.0 / 5.0 + 32.0;
    lcd.print("T:");
    lcd.print(tempF);
    lcd.print("F H:");
    lcd.print(humidity, 1);
    lcd.print("%");
  }


  lcd.setCursor(0,1);
  delay(1000);
  lcd.print("                ");
//change LED and or beep depending on temperature ranges
  if (tempF < lowtemp)
  {
    greenValue = 255;
    redValue = 0;
    blueValue= 0;
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    
  }
  if (tempF < hightemp && tempF > lowtemp)
  {
    redValue = 255;
    greenValue= 255;
    blueValue= 0;
     analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    
  }
  if (tempF > hightemp)
  {
    if (beep_enabled == true)
    {
      tone(speaker, melody[5], counter);
    }
    lcd.setCursor(0,1);
    lcd.print("Temp Alarm hit        ");
    redValue = 255;
    blueValue = 0;
    greenValue = 0;
     analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
   
  }
 }
}

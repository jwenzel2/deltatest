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
int menupin = 22;
int pluspin = 24;
int negativepin = 26;
int okpin = 28;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
//variables for the project
float temperature;
float humidity;
float tempF;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
// establish temp ranges
float lowtemp = 75;
float hightemp = 82;
bool beep_enabled = true;
//speaker settings
int counter = 500;
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int redValue;
int greenValue;
int blueValue;
//menuvariables
int buttonStateMenu = 0; 
int buttonStatePlus = 0;
int buttonStateNegative = 0;
int buttonStateOk = 0;
int currentScreen = 0;
const int numOfScreens = 2;
int parameters[numOfScreens] = {lowtemp, hightemp};
String screens[numOfScreens][2] = {{"Low Temp",String(lowtemp)},{"High Temp",String(hightemp)}};




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
  pinMode(pluspin, INPUT_PULLUP);
  pinMode(negativepin,INPUT_PULLUP);
  pinMode(okpin,INPUT_PULLUP);
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
  if (currentScreen == 2) {
    lcd.setCursor(0,0);
    lcd.print(screens[0][0]);
    lcd.print(": ");
    lcd.setCursor(0,1);
    lcd.print(parameters[0]);

    lcd.setCursor(0,1);
    lcd.print(screens[1][0]);
    lcd.print(": ");
    lcd.setCursor(0,1);
    lcd.print(parameters[1]);
  }
  else 
  {
    lcd.setCursor(0,0);
    lcd.print("MENU TOTORIAL");
    lcd.setCursor(0,2);
    lcd.print(screens[currentScreen][0]);
    lcd.setCursor(0,3);
    lcd.print(parameters[currentScreen]);
    lcd.print(" ");
    lcd.print(screens[currentScreen][1]);
  }
}

void loop()
{
 
  lcd.setCursor(0,0);
  /*
  // menu code for future
    buttonStateMenu = digitalRead(menupin);
    buttonStatePlus = digitalRead(pluspin);
    buttonStateNegative= digitalRead(negativepin);
    buttonStateOk = digitalRead(okpin);

    if(buttonStateMenu == LOW)
    {
      lcd.clear();
      if (currentScreen == 0 )
      {
        currentScreen = numOfScreens -1;
      }
      else 
      {
        currentScreen--;
      }
    }
      else if (buttonStateNegative == LOW){
    lcd.clear();
    parameters[currentScreen]--;
}
    
  else if (buttonStatePlus == LOW){
    lcd.clear();
    parameters[currentScreen]++;
}

  else if (buttonStateOk == LOW){
    lcd.clear();
    if (currentScreen == numOfScreens-1) {
      currentScreen = 0;
    } else {
      currentScreen++;
    }
}
      printScreen();
    delay(200);
    }
*/  
  
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
 // delay(500);
}

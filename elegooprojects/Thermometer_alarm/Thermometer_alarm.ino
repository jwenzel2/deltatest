//www.elegoo.com
//2016.12.9

#include <LiquidCrystal.h>
#include "pitches.h"
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 13;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
int tempPin = 0;
int redValue;
int greenValue;
int blueValue;
int menupin = 22;
int pluspin = 24;
int negativepin = 26;
int okpin = 28;
int buttonStateMenu = 0; 
int buttonStatePlus = 0;
int buttonStateNegative = 0;
int buttonStateOk = 0;
int currentScreen = 0;
const int numOfScreens = 4;
int parameters[numOfScreens] = {0, 10, 50, 100};
String screens[numOfScreens][2] = {{"Value_1","Unit_1"}, {"Value_2", "Unit_2"}, {"Value_3","Unit_3"}, {"Value_4","Unit_4"}};
#define BLUE 5
#define GREEN 4
#define RED 2


//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
void setup()
{
  lcd.begin(16, 2);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  pinMode(menupin, INPUT_PULLUP);
  pinMode(pluspin, INPUT_PULLUP);
  pinMode(negativepin,INPUT_PULLUP);
  pinMode(okpin,INPUT_PULLUP);
}

int ledPin = 2;
int counter = 200;


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


void printScreen() {
  if (currentScreen == 3) {
    lcd.setCursor(0,0);
    lcd.print(screens[0][0]);
    lcd.print(": ");
    lcd.print(parameters[0]);
    lcd.print(" ");
    lcd.print(screens[0][1]);

    lcd.setCursor(0,1);
    lcd.print(screens[1][0]);
    lcd.print(": ");
    lcd.print(parameters[1]);
    lcd.print(" ");
    lcd.print(screens[1][1]);

    lcd.setCursor(0,2);
    lcd.print(screens[2][0]);
    lcd.print(": ");
    lcd.print(parameters[2]);
    lcd.print(" ");
    lcd.print(screens[2][1]);

    lcd.setCursor(0,3);
    lcd.print(screens[3][0]);
    lcd.print(": ");
    lcd.print(parameters[3]);
    lcd.print(" ");
    lcd.print(screens[3][1]);
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("MENU TOTORIAL");
    lcd.setCursor(0,2);
    lcd.print(screens[currentScreen][0]);
    lcd.setCursor(0,3);
    //lcd.print(parameters[currentScreen]);
    lcd.print(" ");
    lcd.print(screens[currentScreen][1]);
  }
}

void loop()
{
  /* menu code for future
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
      printScreen();
    delay(200);
    }
  */
  int tempReading = analogRead(tempPin);
  float temperature;
  float humidity;
  
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    
    float tempVolts = tempReading * 5.0 / 1024.0;
//    float tempC = (tempVolts - 0.5) * 10.0;
//    float tempF = tempC * 9.0 / 5.0 + 32.0;
  

  // Display Temperature in C
  lcd.setCursor(0, 0);
  //lcd.print("Temp         F  ");
  // Display Temperature in F
  //lcd.print("Temp         F  ");
  lcd.setCursor(0, 0);
  // Display Temperature in C
  lcd.print("T:");
  lcd.print(tempF);
  lcd.print("F H:");

  if( measure_environment( &temperature, &humidity ) == true )
  {
    lcd.print(humidity, 1);
    lcd.print("%");
  }

    

  // Display Temperature in F
  //lcd.print(tempF+"F Humid:"+humidity);

  lcd.setCursor(0,1);
  delay(1000);
  lcd.print("                ");

  if (tempF < 75)
  {
    greenValue = 255;
    redValue = 0;
    blueValue= 0;
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
  }
  if (tempF < 82 && tempF > 75)
  {
    redValue = 255;
    greenValue= 255;
    blueValue= 0;
     analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
  }
  if (tempF > 82)
  {
    tone(3, melody[5], counter);
    lcd.setCursor(0,1);
    lcd.print("Temp Alarm hit        ");
    redValue = 255;
    blueValue = 0;
    greenValue = 0;
     analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
  }
  delay(500);
}

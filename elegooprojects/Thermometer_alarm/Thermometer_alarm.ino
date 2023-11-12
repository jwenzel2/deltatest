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
int buttonApin = 22;
int buttonBpin = 24;
int buttonCpin = 26;
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
pinMode(buttonApin, INPUT_PULLUP);
pinMode(buttonBpin, INPUT_PULLUP);
pinMode(buttonCpin,INPUT_PULLUP);
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

void loop()
{
  int tempReading = analogRead(tempPin);
  float temperature;
  float humidity;
  
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  /*  replaced
    float tempVolts = tempReading * 5.0 / 1024.0;
    float tempC = (tempVolts - 0.5) * 10.0;
    float tempF = tempC * 9.0 / 5.0 + 32.0;
  */

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
  
 //test code here
 if (digitalRead(buttonApin) == LOW)
 {
    lcd.setCursor(0, 1);
    lcd.print("              ");
    lcd.setCursor(0, 1);
    lcd.print("button a pressed");
 }

  if (digitalRead(buttonBpin) == LOW)
 {
    lcd.setCursor(0, 1);
    lcd.print("                  ");
    lcd.setCursor(0, 1);
    lcd.print("button b pressed");
 }

 if (digitalRead(buttonCpin) == LOW)
 {
    lcd.setCursor(0, 1);
    lcd.print("                  ");
    lcd.setCursor(0, 1);
    lcd.print("button c pressed");
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

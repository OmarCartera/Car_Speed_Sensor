/**************
 *  CURT FS   *
 * 6/12/2016  *
 *            *
 * Omar Gamal *
 **************/

//defining pins
#define VIRTUAL_GND 12
#define SPEED_SIGNAL 3

//LCD Library
#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 10, 9, 8, 7, 6);

//The radius of our FS wheel in cm.
#define RADIUS 23

//Duration must be unsigned long because micros expand very fast.
unsigned long int duration;

//16-bit variables to hold rpm and kph values is more than enough
unsigned int rpm;
unsigned int kph;
//unsigned int max_speed = 0;


//void printKph();

//void printMax();

void setup()
{
  //another power supply from arduino to get 5 out of the sensor not used with our FS car.
  //works with the power bank.
  //pinMode(9, OUTPUT);
  //analogWrite(9, 400);

  //Setting the virtual ground pin *must check the pin 12*
  pinMode(VIRTUAL_GND, INPUT);//NEVER put it output or you fry the Dueno!

  //Sensor pin
  pinMode(SPEED_SIGNAL, INPUT);

  //Serial communication baud rate
  Serial.begin(9600);

  //LCD Initialization
  lcd.begin(16, 4);
  lcd.clear();

  //Putting the constant characters on the LCD
  lcd.setCursor(8, 1);
  lcd.print("kph");

  lcd.setCursor(4, 1);
  lcd.print(" 0  ");
}

void loop()
{
  //returns the us of the revolution and returns 0 if less than 5.8 rpm
  //or 1 kph
  duration = pulseIn(SPEED_SIGNAL, HIGH, 5202477);

  //If the wheel isn't moving or slower than 1 kph
  if(duration == 0)
  {
    rpm = 0;
    kph = 0;

    lcd.setCursor(4, 1);
    lcd.print(" 0  ");

    lcd.setCursor(8, 1);
    lcd.print("kph");
    
    Serial.println("0   kph");
  }

  //If the wheel is moving any thing higher than 5.8 rpm or 1 kph
  else
  {
    //converting from us to rpm
    rpm = 60000000/(float(duration));

    //converting from rpm to kph
    kph = (rpm * 0.0012 * RADIUS * 3.141592654);

    lcd.setCursor(4, 1);
    lcd.print(kph);
    Serial.println(kph);

    lcd.setCursor(8, 1);
    lcd.print("kph");
  }

  lcd.setCursor(1, 0);
  lcd.print(digitalRead(SPEED_SIGNAL));

  
  //should be optimized like the high performance cars and changed to millis() to free the processor
  delay(100);
}

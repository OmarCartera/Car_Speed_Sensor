/**************
 *  CURT FS   *
 * 18/4/2016  *
 *            *
 * Omar Gamal *
 **************/

//testing timers to hold the processors before counting
int timer1 = 0;
int timer2 = 0;

//The tyre outer radius of our FS wheel in cm.
#define RADIUS 23

//virtual ground pin, should be revised
#define VIRTUAL_GND 12

//SPEED_SIGNAL = wheel speed hall effect sensor, RPM_SIGNAL = rpm from engine ECU
#define SPEED_SIGNAL 3
#define RPM_SIGNAL 5


//LCD Library and configurations to 4-bit data mode
#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 10, 9, 8, 7, 6);

//durations must be unsigned long because micros expand very quickly.
unsigned long int speed_duration;
unsigned long int rpm_duration;

//16-bit variables to hold rpm, kph, max speed and engine rpm values is more than enough. max of 65536
unsigned int wheel_rpm;
unsigned int kph;
unsigned int max_speed = 0;
unsigned int engine_rpm = 0;


//setup and configurations function
void setup()
{
  //another power supply from arduino to get 5 out of the sensor not used with our FS car.
  //works with the power bank.
  //pinMode(9, OUTPUT);
  //analogWrite(9, 400);

  //Setting the virtual ground pin *must check the pin 12*
  pinMode(VIRTUAL_GND, INPUT);//NEVER put it output or you fry the Dueno!

  //Sensors pin configurations
  pinMode(SPEED_SIGNAL, INPUT);
  pinMode(RPM_SIGNAL, INPUT);

  //Serial communication baud rate
  Serial.begin(9600);

  //LCD Initialization
  lcd.begin(16, 4);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   0     RPM    ");

  
  //Putting the constant characters on the LCD
  lcd.setCursor(9, 2);
  lcd.print("kph");

  lcd.setCursor(3, 2);
  lcd.print("  0  ");
}

void loop()
{
  //returns the us of the revolution and returns 0 if less than 5.8 rpm
  //or 1 kph
  while((digitalRead(SPEED_SIGNAL) == HIGH) && (timer1 < 10000))
  {
    timer1++;
  }
  
  speed_duration = pulseIn(SPEED_SIGNAL, HIGH, 5202477);

  timer1 = 0;
  //If the wheel isn't moving or slower than 1 kph
  if(speed_duration == 0)
  {
    wheel_rpm = 0;
    kph = 0;

    lcd.setCursor(0, 2);
    lcd.print("     0   kph    ");

    Serial.print("SPEED   ");
    Serial.println(kph);
  }

  //If the wheel is moving any thing higher than 5.8 rpm or 1 kph
  else
  {
    //converting from us to rpm
    wheel_rpm = 60000000/(float(speed_duration));

    //converting from rpm to kph
    kph = (wheel_rpm * 0.0012 * RADIUS * 3.141592654);

    lcd.setCursor(0, 2);
    lcd.print("     0   kph    ");
    
    lcd.setCursor(5, 2);
    lcd.print(kph);

    Serial.print("SPEED   ");
    Serial.println(kph);
  }

  max_speed = (max_speed > kph ? max_speed : kph);
  
  lcd.setCursor(0, 3);
  lcd.print(max_speed);


  //engine rpm calculations
  while((digitalRead(RPM_SIGNAL) == HIGH) && (timer2 < 10000))
  {
    timer2++;
  }  

  rpm_duration = pulseIn(RPM_SIGNAL, HIGH);
  timer2 = 0;
  
  if (rpm_duration != 0)
  {
    //converting from us to rpm. Multiplied by 1/2 because ECU returns double the RPM
    engine_rpm = (0.5 * 60000000)/(float(rpm_duration));

    lcd.setCursor(0, 0);
    lcd.print("         RPM    ");
    
    lcd.setCursor(3, 0);
    lcd.print(engine_rpm);

    Serial.print("RPM   ");
    Serial.println(engine_rpm);
  }

  else
  { 
    lcd.setCursor(0, 0);
    lcd.print("   0     RPM    ");

    Serial.print("RPM   ");
    Serial.println(engine_rpm);
  }
  
  //should be optimized like the high performance cars and changed to millis() to free the processor
  delay(50);
}

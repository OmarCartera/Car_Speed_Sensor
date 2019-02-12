/****************
 *   CURT FS    *
 *  14/06/2017  *
 *              *
 * Omar Cartera *
 ****************/

//gear ratios (input:output) combined ratio for each gear, its ratio + final drive ratio
float final_drive = 0.355;    //11:31

float gear_1 = 0.389 * final_drive;         //14:36
float gear_2 = 0.531 * final_drive;         //17:32
float gear_3 = 0.679 * final_drive;         //19:28
float gear_4 = 0.846 * final_drive;         //22:26
float gear_5 = 1.043 * final_drive;         //24:23
float gear_6 = 1.238 * final_drive;         //26:21

float ratio = 0;

int current_gear = 0;

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


  /*
  //Putting the constant characters on the LCD
  lcd.setCursor(9, 2);
  lcd.print("kph");

  lcd.setCursor(3, 2);
  lcd.print("  0  ");
  */
}

void loop()
{
  //returns the us of the revolution and returns 0 if less than 5.8 rpm
  //or 1 kph
  
  speed_duration = pulseIn(SPEED_SIGNAL, HIGH, 5202477);

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

    //getting the current gear given engine rpm and wheel rpm
    ratio = float(wheel_rpm)/float(engine_rpm);

    if (ratio <= gear_1)
    {
      current_gear = 1;
    }

    else if (ratio < gear_2)
    {
      current_gear = 2;
    }
    
    else if (ratio < gear_3)
    {
      current_gear = 3;
    }

    else if (ratio < gear_4)
    {
      current_gear = 4;
    }

    else if (ratio < gear_5)
    {
      current_gear = 5;
    }

    else if (ratio < gear_6)
    {
      current_gear = 6;
    }
  }

  //print the current gear on the lcd

  
  max_speed = (max_speed > kph ? max_speed : kph);
  
  lcd.setCursor(0, 3);
  lcd.print(max_speed);

  delay(50);


  /////////////////////////////////////////////////////////////////////////////
  //engine rpm calculations
  rpm_duration = pulseIn(RPM_SIGNAL, HIGH);
  
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
    Serial.println(0);
  }

  //output speed/input speed = ratio;
  //this ratio will be compared to the gear ratios
  //the closer ratio to it will be the current gear
  //should be optimized like the high performance cars and changed to millis() to free the processor
  delay(100);
}

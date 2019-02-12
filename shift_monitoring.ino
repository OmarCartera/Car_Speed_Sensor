/*************************
* Gear shift indicator + *
*    quick shifter       *
*    By: Omar Gamal      *
* c.omargamal@gmail.com  *
*                        *
*       17/4/2016        *
**************************/


//let power-button-resistor-ground and take the output at
//button and resistor mutual pin


//defining constants for shift up/down buttons
#define upButton 11
#define downButton 12
#define relay 10


//defining the 7-segment pins
#define a 7
#define b 8
#define c 2
#define d 3
#define e 4
#define f 5
#define g 6


//6 speed gearbox
#define maxShifts 6
#define minShifts 0


//defining boolean variables to save last button press
boolean lastUpButton = LOW;
boolean lastDownButton = LOW;


//the number of gear we're in
int i=0;


//controls the light of the 7-segment
void light(int number)
{
  switch (number)
  {
    case 0:
      digitalWrite(a, HIGH);
      digitalWrite(b, HIGH);
      digitalWrite(c, HIGH);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, HIGH);
      digitalWrite(g, LOW);
      break;

    case 1:
      digitalWrite(a, LOW);
      digitalWrite(b, HIGH);
      digitalWrite(c, HIGH);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;

    case 2:
      digitalWrite(a, HIGH);
      digitalWrite(b, HIGH);
      digitalWrite(c, LOW);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, LOW);
      digitalWrite(g, HIGH);
      break;

    case 3:
      digitalWrite(a, HIGH);
      digitalWrite(b, HIGH);
      digitalWrite(c, HIGH);
      digitalWrite(d, HIGH);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, HIGH);
      break;

    case 4:
      digitalWrite(a, LOW);
      digitalWrite(b, HIGH);
      digitalWrite(c, HIGH);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, HIGH);
      digitalWrite(g, HIGH);
      break;

    case 5:
      digitalWrite(a, HIGH);
      digitalWrite(b, LOW);
      digitalWrite(c, HIGH);
      digitalWrite(d, HIGH);
      digitalWrite(e, LOW);
      digitalWrite(f, HIGH);
      digitalWrite(g, HIGH);
      break;

    case 6:
      digitalWrite(a, HIGH);
      digitalWrite(b, LOW);
      digitalWrite(c, HIGH);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, HIGH);
      digitalWrite(g, HIGH);
      break;
  }
}





//a function that shifts up
void upShift()
{
  if(digitalRead(upButton) == lastUpButton)
    {
      if(i<maxShifts)
      {
        digitalWrite(relay, HIGH);
        Serial.println(++i);
        light(i);
        delay(200);
      }
    }


  while(digitalRead(upButton) == HIGH)
  {
    //it stays here until you release the button
  }

  //to prevent shifting more than once
  delay(5);
  digitalWrite(relay, LOW);
}





//a function that shifts down
void downShift()
{
  if(digitalRead(downButton) == lastDownButton)
    {
      if(i==1)
      {
        digitalWrite(relay, HIGH);
        Serial.println('N');
        light(0);
        delay(200);
        i--;
      }
      
      if(i>(minShifts+1))
      {
        digitalWrite(relay, HIGH);
        Serial.println(--i);
        light(i);
        delay(200);
      }
    }

    
  while(digitalRead(downButton) == HIGH)
  {
    //it stays here until you release the button
  }

  //to prevent shifting more than once
  delay(5);
  digitalWrite(relay, LOW);
}





void setup()
{
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(relay, OUTPUT);
  
  Serial.begin(9600);
  Serial.println('N');
  light(0);

  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(g,OUTPUT);
}





void loop()
{    
  lastUpButton = digitalRead(upButton);
  lastDownButton = digitalRead(downButton);

  //if upshift button is pressed
  if(lastUpButton == HIGH)
  {
    upShift();
  }


  //if downshift button is pressed
  else if(lastDownButton == HIGH)
  {
    downShift();
  }
}


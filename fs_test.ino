//use timers instead of micros();
//use millis instead delay();


unsigned int timer = 0;
unsigned int timers = 0;

int engine_rpm = 0;
int kph = 0;

void setup()
{
  /*
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  */

  pinMode(3, INPUT);
  pinMode(5, INPUT);

  digitalWrite(3, LOW);
  digitalWrite(5, LOW);

  Serial.begin(9600);
}

void loop()
{
  //this part is for setting sensors simulators
  /*
  digitalWrite(8, HIGH);
  //delay(5);
  delay(33);
  digitalWrite(8, LOW);
  delay(1);
  */

  while(digitalRead(5) == HIGH);
  while(digitalRead(5) == LOW);
  timer = micros();
  while(digitalRead(5) == HIGH);
  timer = micros() - timer;

  engine_rpm = (0.5 * 60000000)/(float(timer));
  Serial.print(engine_rpm);

  Serial.print("      ");

  while(digitalRead(3) == HIGH);
  while(digitalRead(3) == LOW);
  timers = micros();
  while(digitalRead(3) == HIGH);
  timers = micros() - timers;
  
  kph = (60000000/(float(timers)) * 0.08670795725);
  Serial.println(kph);
  
  delay(24);
}

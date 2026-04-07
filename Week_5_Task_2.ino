const int relay1 = 7;
const int relay2 = 8;
const int limitSwitch = 2;
const int led = 13;

void setup() 
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  pinMode(limitSwitch, INPUT_PULLUP); 
  pinMode(led, OUTPUT);
}
void loop() 
{
  extendActuator();
  while (digitalRead(limitSwitch) == HIGH) 
  {
  }
  stopActuator();
  digitalWrite(led, HIGH); // Turn ON LED
  delay(5000);
  retractActuator();
  delay(5000);
  digitalWrite(led,LOW);
}

void extendActuator()
{
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,LOW);
}

void retractActuator()
{
  digitalWrite(relay1,LOW);
  digitalWrite(relay2,HIGH);
}

void stopActuator()
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}




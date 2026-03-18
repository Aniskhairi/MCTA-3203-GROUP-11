// Define the pins for each segment (D2 to D8)
const int segmentA = 7; // D7
const int segmentB = 6; // D6
const int segmentC = 4; // D4
const int segmentD = 3; // D3
const int segmentE = 2; // D2
const int segmentF = 8; // D8
const int segmentG = 5; // D5


// Define the button
const int inc_button = 10;
const int reset_button = 11;
int increment = 0;
int reset = 0;
int count = 0;


void setup() {
// Initialize the digital pins as OUTPUTs & INPUTs
pinMode(segmentA, OUTPUT);
pinMode(segmentB, OUTPUT);
pinMode(segmentC, OUTPUT);
pinMode(segmentD, OUTPUT);
pinMode(segmentE, OUTPUT);
pinMode(segmentF, OUTPUT);
pinMode(segmentG, OUTPUT);
pinMode(inc_button, INPUT_PULLUP);
pinMode(reset_button, INPUT_PULLUP);
}


void loop() {
  increment = digitalRead(inc_button);
  reset = digitalRead(reset_button);


  if (reset == LOW){
    count = 0;
    display(count);
    delay(200);
  }


  if (increment == LOW){
    count++;
    if (count>9){
      count = 0;
    }
    display(count);
    delay(200);
  }
}


void display(int d){
  switch(d){
    case 0:
     digitalWrite(segmentA, HIGH);
     digitalWrite(segmentB, HIGH);
     digitalWrite(segmentC, HIGH);
     digitalWrite(segmentD, HIGH);
     digitalWrite(segmentE, HIGH);
     digitalWrite(segmentF, HIGH);
     digitalWrite(segmentG, LOW);
     break;


    case 1:  
     digitalWrite(segmentA, LOW);
     digitalWrite(segmentB, HIGH);
     digitalWrite(segmentC, HIGH);
     digitalWrite(segmentD, LOW);
     digitalWrite(segmentE, LOW);
     digitalWrite(segmentF, LOW);
     digitalWrite(segmentG, LOW);
     break;


    case 2:  
     digitalWrite(segmentA, HIGH);
     digitalWrite(segmentB, HIGH);
     digitalWrite(segmentG, HIGH);
     digitalWrite(segmentE, HIGH);
     digitalWrite(segmentD, HIGH);
     digitalWrite(segmentC, LOW);
     digitalWrite(segmentF, LOW);
     break;


    case 3:
     digitalWrite(segmentA, HIGH);
     digitalWrite(segmentB, HIGH);
     digitalWrite(segmentC, HIGH);
     digitalWrite(segmentD, HIGH);
     digitalWrite(segmentG, HIGH);
     digitalWrite(segmentE, LOW);
     digitalWrite(segmentF, LOW);
     break;


    case 4:
     digitalWrite(segmentB, HIGH);
     digitalWrite(segmentF, HIGH);
     digitalWrite(segmentG, HIGH);
     digitalWrite(segmentC, HIGH);
     digitalWrite(segmentA, LOW);
     digitalWrite(segmentD, LOW);
     digitalWrite(segmentE, LOW);
     break;


    case 5:
     digitalWrite(segmentA, HIGH);
     digitalWrite(segmentF, HIGH);
     digitalWrite(segmentG, HIGH);
     digitalWrite(segmentC, HIGH);
     digitalWrite(segmentD, HIGH);
     digitalWrite(segmentB, LOW);
     digitalWrite(segmentE, LOW);
     break;


    case 6:
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentE, HIGH);
      digitalWrite(segmentB, LOW);
      break;


    case 7:
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, LOW);
      break;


    case 8:
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentE, HIGH);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, HIGH);
      break;


    case 9:
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, HIGH);
      digitalWrite(segmentE, LOW);
      break;
}
}

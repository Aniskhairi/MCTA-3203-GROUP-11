#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;
Servo gateServo;

const int greenLedPin = 5; 
const int servoPin = 6;    
const int redLedPin = 7;   

const unsigned long streamDurationMs = 2000; // Stream data for 2 seconds
const unsigned long sampleDelayMs = 50;      // 20 Hz sample rate

void setup() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  
  gateServo.attach(servoPin);
  gateServo.write(0); // Start at closed position
  
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  
  // Default state: red LED ON (idle/unauthorized)
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);
}

void openGate() {
  for (int pos = 90; pos <= 180; pos += 2) {
    gateServo.write(pos);
    delay(15);
  }
  Serial.println("Servo opened (90 -> 180)");
}

void closeGate() {
  for (int pos = 180; pos >= 90; pos -= 2) {
    gateServo.write(pos);
    delay(15);
  }
  Serial.println("Servo closed (180 -> 90)");
}

void wrongCardFlash() {
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);
  delay(100);
  // Blink red LED 3 times
  for (int i = 0; i < 3; i++) {
    digitalWrite(redLedPin, HIGH);
    delay(150);
    digitalWrite(redLedPin, LOW);
    delay(150);
  }
  digitalWrite(redLedPin, HIGH); // Return to idle red ON
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    
    if (cmd == '1') {
      // Access granted
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
      openGate();
    } 
    else if (cmd == '0') {
      // System disarmed (Lock)
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
      closeGate();
    } 
    else if (cmd == 'X') {
      // Wrong card feedback
      wrongCardFlash();
    } 
    else if (cmd == 'R') {
      // Python requested motion data
      unsigned long start = millis();
      while (millis() - start < streamDurationMs) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        Serial.print(ax); Serial.print(',');
        Serial.print(ay); Serial.print(',');
        Serial.print(az); Serial.print(',');
        Serial.print(gx); Serial.print(',');
        Serial.print(gy); Serial.print(',');
        Serial.println(gz);
        
        delay(sampleDelayMs);
      }
    }
  }
}


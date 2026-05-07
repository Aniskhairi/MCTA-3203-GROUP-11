#include "BluetoothSerial.h"
#include <ESP32Servo.h>

BluetoothSerial SerialBT;
Servo myServo;

const int potPin = 34;   // Potentiometer input
const int servoPin = 13; // Servo signal pin
int potValue = 0;

void setup() {
  Serial.begin(9600);                   
  SerialBT.begin("Group_11"); // Change to your group name
  
  myServo.attach(servoPin); // Attaching the servo
  
  Serial.println("Bluetooth and Servo initialized.");
}

void loop() {
  // Read sensor value
  potValue = analogRead(potPin);

  // Logic for Servo positions based on input ranges
  if (potValue >= 0 && potValue < 1365) {
    myServo.write(0);
  } 
  else if (potValue >= 1365 && potValue < 2730) {
    myServo.write(90);
  } 
  else if (potValue >= 2730 && potValue <= 4095) { // Adjusted for standard 10-bit range
    myServo.write(180);
  }

  // Send data to Smartphone via Bluetooth
  SerialBT.print("Sensor Value: ");
  SerialBT.println(potValue);
  
  // Also print to local Serial Monitor for debugging
  Serial.print("PotValue: ");
  Serial.println(potValue);

  delay(500); // Small delay for stability
}





#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX
// Define the LED pin
const int ledPin = 13;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED off
  // Wait for HuskyLens to connect
  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed! Check wiring and Protocol (Serial 9600)"));
    delay(500);
  }
  Serial.println("HuskyLens Ready!");
}

void loop() {
  // 1. Request data from HuskyLens
  if (!huskylens.request()) {
    // If the request fails or sees nothing, turn the LED off
    digitalWrite(ledPin, LOW);
    return;
  }
  // 2. Check if any trained objects are in view
  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();
    // 3. Check if the detected object is "Object 1"
    if (result.command == COMMAND_RETURN_BLOCK && result.ID == 1) {
      Serial.println("Object 1 detected! LED ON");
      digitalWrite(ledPin, HIGH);
    } 
    else {
      // If a different object is seen, turn LED off
      digitalWrite(ledPin, LOW);
    }
  } 
  else {
    // 4. If no objects are available in the frame, turn LED off
    digitalWrite(ledPin, LOW);
  }
}


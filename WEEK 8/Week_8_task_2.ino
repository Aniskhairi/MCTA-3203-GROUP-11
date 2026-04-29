#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX

// RGB LED Pins (PWM capable)
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  // Set RGB pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed! Check Protocol (Serial 9600)"));
    delay(500);
  }
}

void loop() {
  if (!huskylens.request()) {
    setColor(0, 0, 0); 
    return;
  }

  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();

    // The Serial Monitor will now show exactly which color is triggered
    Serial.print("Detected ID: ");
    Serial.print(result.ID);
    Serial.print(" - Color: ");

    switch (result.ID) {
      case 1:
        Serial.println("RED");
        setColor(255, 0, 0);
        break;

      case 2:
        Serial.println("GREEN");
        setColor(0, 255, 0);
        break;

      case 3:
        Serial.println("BLUE");
        setColor(0, 0, 255);
        break;

      case 4:
        Serial.println("YELLOW");
        setColor(255, 255, 0);
        break;

      case 5:
        Serial.println("PURPLE");
        setColor(150, 0, 255); 
        break;

      default:
        Serial.println("UNKNOWN/WHITE");
        setColor(255, 255, 255);
        break;
    }
  } else {
    // Optional: Print "Searching..." if you want to see activity when empty
    setColor(0, 0, 0);
  }
}

// Helper function to handle PWM values
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}



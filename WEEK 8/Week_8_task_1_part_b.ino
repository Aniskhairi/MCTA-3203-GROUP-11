#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  
  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed!"));
    delay(500);
  }
  Serial.println("HuskyLens Color Recognition Ready!");
}

void loop() {
  if (!huskylens.request()) return;
  
  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();
    
    if (result.command == COMMAND_RETURN_BLOCK) {
      // Evaluate the ID and print the corresponding color
      switch (result.ID) {
        case 1:
          Serial.println("Color RED is detected");
          break;
        case 2:
          Serial.println("Color BLUE is detected");
          break;
        case 3:
          Serial.println("Color GREEN is detected");
          break;
        default:
          Serial.println("Unknown Color detected");
          break;
      }
    }
  }
}


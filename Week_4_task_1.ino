#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// --- DEFINE YOUR LED PIN HERE ---
const int ledPin = 7; // Fill in 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED off
}

void loop() {
  // 1. Read motion data
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // 2. Send formatted data to Python (ax,ay,az)
  Serial.print(ax); Serial.print(',');
  Serial.print(ay); Serial.print(',');
  Serial.println(az);

  // 3. Listen for commands from Python to control the LED
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'O') {
      digitalWrite(ledPin, HIGH); // Turn LED ON
    } 
    else if (cmd == 'F') {
      digitalWrite(ledPin, LOW);  // Turn LED OFF
    }
  }
  
  delay(50); // Small delay for stability
}

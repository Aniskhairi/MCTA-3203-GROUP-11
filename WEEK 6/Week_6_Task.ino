#include "esp_camera.h"
#include <WiFi.h>
#include <ESP32Servo.h> // Library untuk Servo

// ===========================
// Select camera model
// ===========================
#define CAMERA_MODEL_AI_THINKER // PENTING: Untuk ESP32-CAM AI-Thinker
#include "camera_pins.h"

// ===========================
// WiFi Credentials
// ===========================
const char *ssid = "Anis's";
const char *password = "00000000";

// ===========================
// Servo & Button Configuration
// ===========================
const int PIN_SERVO = 14; 
const int PIN_BUTANG = 13; // Sesuai arahan tugasan

Servo myServo;
int posisi = 0;    
int arah = 1;

void startCameraServer();
void setupLedFlash();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // --- Setup Pushbutton & Servo ---
  pinMode(PIN_BUTANG, INPUT_PULLUP); // Gunakan internal pull-up
  ESP32PWM::allocateTimer(3);        // Allocate timer untuk PWM
  myServo.setPeriodHertz(50);        // Servo standard 50Hz
  myServo.attach(PIN_SERVO, 600, 2400);
  myServo.write(posisi);
  // --------------------------------

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(LED_GPIO_NUM)
  setupLedFlash();
#endif

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  // --- Mandatory Task: Manual Control Panning ---
  if (digitalRead(PIN_BUTANG) == LOW) {
    Serial.println("Butang dikesan! Menggerakkan servo..."); 
    // ... kod gerak servo ...
  }

  // Servo hanya bergerak bila butang ditekan (LOW)
  if (digitalRead(PIN_BUTANG) == LOW) {
    posisi += arah;
    
    // Pusing balik bila sampai had 180 atau 0
    if (posisi >= 180 || posisi <= 0) {
      arah *= -1;
    }
    
    myServo.write(posisi);
    delay(15); // Kelajuan panning. Kecilkan nombor untuk lebih laju.
  }
}
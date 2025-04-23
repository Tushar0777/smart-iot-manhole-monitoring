#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <MPU6050.h>

// Wi-Fi Credentials
const char* ssid = "NSUT_WIFI";
const char* password = "";

// Sensor Pins
#define TRIG_PIN 19
#define ECHO_PIN 21
#define TILT1_PIN 18
#define TILT2_PIN 35
#define GAS_PIN 34

// Thresholds
#define DIST_THRESHOLD 100
#define GAS_THRESHOLD 1500

WebServer server(80);
MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Booting...");

  // Pin Modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TILT1_PIN, INPUT);
  pinMode(TILT2_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);

  // Initialize MPU6050 (on custom I2C pins)
  Wire.begin(25, 26);
  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  // WiFi Connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout-- > 0) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed!");
    while (1);
  }

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "ESP32 Manhole Monitoring - Access /data for JSON");
}

void handleData() {
  // --- Ultrasonic Distance ---
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.034 / 2;

  // --- Tilt Sensors ---
  int tilt1 = digitalRead(TILT1_PIN);
  int tilt2 = digitalRead(TILT2_PIN);

  // --- Gas Sensor ---
  int gasValue = analogRead(GAS_PIN);

  // --- MPU6050 Data ---
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // --- Debug Output ---
  Serial.print("Distance: "); Serial.print(distance); Serial.print(" cm | ");
  Serial.print("Tilt1: "); Serial.print(tilt1); Serial.print(" | ");
  Serial.print("Tilt2: "); Serial.print(tilt2); Serial.print(" | ");
  Serial.print("Gas: "); Serial.print(gasValue); Serial.print(" | ");
  Serial.print("Accel: "); Serial.print(ax); Serial.print(" "); Serial.print(ay); Serial.print(" "); Serial.print(az); Serial.print(" | ");
  Serial.print("Gyro: "); Serial.print(gx); Serial.print(" "); Serial.print(gy); Serial.print(" "); Serial.println(gz);

  // --- JSON Response ---
  String json = "{";
  json += "\"distance\":" + String(distance, 1) + ",";
  json += "\"tilt1\":" + String(tilt1) + ",";
  json += "\"tilt2\":" + String(tilt2) + ",";
  json += "\"gas\":" + String(gasValue) + ",";
  json += "\"accel\":[" + String(ax) + "," + String(ay) + "," + String(az) + "],";
  json += "\"gyro\":[" + String(gx) + "," + String(gy) + "," + String(gz) + "]";
  json += "}";

  server.send(200, "application/json", json);
}


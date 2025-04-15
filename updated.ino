#include <WiFi.h>
#include <WebServer.h>

// Pin Definitions
#define TRIG_PIN 19
#define ECHO_PIN 21
#define TILT1_PIN 18
#define GAS_PIN 34

#define DIST_THRESHOLD 20
#define GAS_THRESHOLD 400

// Replace with your Wi-Fi credentials
const char* ssid = "NSUT_WIFI";
const char* password = ""; // Leave blank if open network

WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Booting...");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TILT1_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);

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
    while (true); // Stay here
  }

  server.on("/", handleRoot);
  server.on("/data", handleData);  // JSON endpoint
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "ESP32 Manhole Monitoring - Access /data for JSON");
}

// JSON Data Endpoint
void handleData() {
  // Ultrasonic distance calculation
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout after 30ms
  float distance = duration * 0.034 / 2;

  int tilt = digitalRead(TILT1_PIN);
  int gas = analogRead(GAS_PIN);

  // Print to Serial for debug
  Serial.print("Distance: "); Serial.print(distance); Serial.print(" cm | ");
  Serial.print("Tilt: "); Serial.print(tilt); Serial.print(" | ");
  Serial.print("Gas: "); Serial.println(gas);

  // Create JSON string
  String json = "{";
  json += "\"distance\":" + String(distance, 1) + ",";
  json += "\"tilt\":" + String(tilt) + ",";
  json += "\"gas\":" + String(gas);
  json += "}";

  server.send(200, "application/json", json);
}

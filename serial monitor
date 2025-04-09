// Pin Definitions
#define TRIG_PIN 19
#define ECHO_PIN 21
#define TILT1_PIN 18
#define TILT2_PIN 35
#define GAS_PIN 34
#define WATER_PIN 33  // NEW: Water level sensor connected to GPIO33

// Thresholds
#define DIST_THRESHOLD 100 // cm
#define GAS_THRESHOLD 1500 // Adjust based on your sensor
#define WATER_THRESHOLD 1000 // Adjust based on your sensor output

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TILT1_PIN, INPUT);
  pinMode(TILT2_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(WATER_PIN, INPUT);  // NEW: Set water sensor pin as input
}

void loop() {
  // Ultrasonic Distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  // Tilt Sensors
  int tilt1 = digitalRead(TILT1_PIN);
  int tilt2 = digitalRead(TILT2_PIN);

  // Gas Sensor
  int gasValue = analogRead(GAS_PIN);

  // Water Level Sensor
  int waterLevel = analogRead(WATER_PIN);  // NEW

  // Output status
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Tilt1: ");
  Serial.print(tilt1);
  Serial.print(" | Tilt2: ");
  Serial.print(tilt2);
  Serial.print(" | Gas: ");
  Serial.print(gasValue);
  Serial.print(" | Water: ");  // NEW
  Serial.println(waterLevel); // NEW

  // Detection Conditions
  if ((distance > DIST_THRESHOLD) && (tilt2 == LOW)) {
    Serial.println("Manhole OPEN!");
  }

  if (tilt2 == HIGH) {
    Serial.println("No tilt detected");
  }

  if (tilt2 == LOW) {
    Serial.println("TILT DETECTED");
  }

  if (gasValue > GAS_THRESHOLD) {
    Serial.println("Gas Leak Detected!");
  }

  if (waterLevel > WATER_THRESHOLD) {
    Serial.println("Water Level High! Possible Flood Detected.");
  }

  delay(1000);
}

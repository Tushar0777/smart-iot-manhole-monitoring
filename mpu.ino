#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  // Use GPIO23 for SDA and GPIO5 for SCL
  Wire.begin(23, 5);

  Serial.println("Initializing MPU6050...");

  if (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("MPU6050 not found. Check wiring.");
    while (1);
  }

  Serial.println("MPU6050 initialized successfully.");
}

void loop() {
  Vector acc = mpu.readRawAccel();
  Vector gyro = mpu.readRawGyro();

  Serial.print("Accel => X: "); Serial.print(acc.XAxis);
  Serial.print(" Y: "); Serial.print(acc.YAxis);
  Serial.print(" Z: "); Serial.print(acc.ZAxis);

  Serial.print(" | Gyro => X: "); Serial.print(gyro.XAxis);
  Serial.print(" Y: "); Serial.print(gyro.YAxis);
  Serial.print(" Z: "); Serial.println(gyro.ZAxis);

  delay(500);
}

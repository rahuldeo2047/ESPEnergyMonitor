#include <Arduino.h>

//#include <MPU6050_tockn.h>
#include <Wire.h>

#include "I2CScanner.h"

I2CScanner scanner;
//MPU6050 mpu6050(Wire);

//void setup_mpu();
//void loop_mpu();

long timer = 0;

void _setup()
{
  Serial.begin(115200);

  setup_mpu();

  // Wire.begin();
  // Serial.println("=======================================================\n");
  // scanner.Scan();
  // Serial.println("=======================================================\n");

  // mpu6050.begin();
  // mpu6050.calcGyroOffsets(true);
  //	scanner.Init();
}

void _loop()
{
  //mpu6050.update();

  if (millis() - timer > 500)
  {

    loop_mpu();

    // Serial.println("=======================================================");
    // Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    // Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    // Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    // Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());

    // Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
    // Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
    // Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());

    // Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
    // Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());

    // Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
    // Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
    // Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());

    // Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
    // Serial.print("\tangleY : ");Serial.print(mpu6050.getAngleY());
    // Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
    Serial.println("=======================================================\n");
    scanner.Scan();
    Serial.println("=======================================================\n");

    timer = millis();
  }
}

// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
const int MPU_addr = 0x68; // I2C address of the MPU-6050
float Am, Tmpf, Am_norm;
void setup_mpu()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //Serial.begin(9600);
}

float getAccMag()
{
  return Am;
}

// float getAccMagNorm()
// {
//   return Am_norm;
// }

float getTemp()
{
  return Tmpf;
}
bool loop_mpu()
{

  bool status = scanner.Scan(0x68);

  int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

  if (status == true)

  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
    AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    // 16384 @ 2g default settings
    // 2.0/32768.0 mG

    if (isnan(AcX))
    {
      AcX = 0.0;
    }

    if (isnan(AcY))
    {
      AcY = 0.0;
    }

    if (isnan(AcZ))
    {
      AcZ = 0.0;
    }

    float Ax = (float)AcX * 2.0 / 32768.0;
    float Ay = (float)AcY * 2.0 / 32768.0;
    float Az = (float)AcZ * 2.0 / 32768.0;

    if (isnan(Ax))
    {
      Ax = 0.0;
    }

    if (isnan(Ay))
    {
      Ay = 0.0;
    }

    if (isnan(Az))
    {
      Az = 0.0;
    }

    Tmpf = Tmp / 340.00 + 36.53;

    Am = sqrtf(Ax * Ax + Ay * Ay + Az * Az);
 
    if (isnan(Am))
    {
      Am = 0.0;
    }


    Ax = Ax / Am;
    Ay = Ay / Am;
    Az = Az / Am;
     
    Am_norm = sqrtf(Ax * Ax + Ay * Ay + Az * Az);

    if (isnan(Am_norm))
    {
      Am_norm = 0.0;
    }

  }

  //Serial.print("Acm = "); Serial.print(Am, 4);
  //Serial.print(" | AcX = "); Serial.print(Ax, 4);
  //Serial.print(" | AcY = "); Serial.print(Ay, 4);
  //Serial.print(" | AcZ = "); Serial.print(Az, 4);
  //Serial.print(" | Tmp = "); Serial.println(Tmpf);  //equation for temperature in degrees C from datasheet
  //Serial.print(" | GyX = "); Serial.print(GyX);
  //Serial.print(" | GyY = "); Serial.print(GyY);
  //Serial.print(" | GyZ = "); Serial.println(GyZ);
  //delay(333);

  //Serial.println("=======================================================\n");
  return status;
  // Serial.println("=======================================================\n");
}
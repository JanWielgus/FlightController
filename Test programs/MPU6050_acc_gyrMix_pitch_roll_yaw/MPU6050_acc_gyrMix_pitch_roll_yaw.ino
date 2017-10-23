/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Pitch & Roll & Yaw Gyroscope Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Timers
unsigned long timer = 0;
unsigned long timerPrev = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;

float pitchAcc = 0;
float rollAcc = 0;

void setup() 
{
  Serial.begin(115200);

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  //mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  //mpu.setThreshold(3);
}

void loop()
{
  timerPrev = timer;
  timer = micros();
  timeStep = float(timer - timerPrev) / 1000000.0;

  // Read normalized values
  Vector norm = mpu.readNormalizeGyro();
  Vector normAccel = mpu.readNormalizeAccel();

  // Calculate Pitch, Roll and Yaw (Gyro)
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  // Calculate Pitch and Roll (Acc)
  pitchAcc = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  rollAcc = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;

  //-- Filtr komplementarny --
  pitch = 0.98*pitch + 0.02*pitchAcc;
  roll = 0.98*roll + 0.02*rollAcc;

  // Output (po filtrze)
  Serial.print(" Pitch = ");
  Serial.print(pitch);
  Serial.print(" Roll = ");
  Serial.print(roll);  
  Serial.print(" Yaw = ");
  Serial.println(yaw);

/*
  // Pokazanie odczytów tylko acc
  Serial.print("\t");
  Serial.print(pitchAcc);
  Serial.print("\t");
  Serial.println(rollAcc);*/

  // Wait to full timeStep period
  //delay((timeStep*1000) - (millis() - timer));
}

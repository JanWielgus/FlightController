// Author: Jan Wielgus
// Date: 25.10.2017
// 

#include "Sensors.h"
SensorsClass sensors;



void SensorsClass::init()
{
	while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
	{
		//Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
		delay(500);
	}
	//mpu.setThreshold(3);
}



void SensorsClass::readAngles()
{
	//timerPrev = timer;
	//timer = micros();
	//dt_ = float(timer - timerPrev) / 1000000.0;
	
	// Read normalized values
	Vector norm = mpu.readNormalizeGyro();
	Vector normAccel = mpu.readNormalizeAccel();
	
	// Calculate Pitch, Roll and Yaw (Gyro)
	pitch = pitch + norm.YAxis * dt_;
	roll = roll + norm.XAxis * dt_;
	yaw = yaw + norm.ZAxis * dt_;

	// Calculate Pitch and Roll (Acc)
	pitchAcc = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
	rollAcc = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
	
	//-- Filtr komplementarny --
	pitch = 0.98*pitch + 0.02*pitchAcc;
	roll = 0.98*roll + 0.02*rollAcc;
}



void SensorsClass::updateDeltaTime()
{
	uint32_t nowTime = micros();
	dt_ = float(nowTime - timerPrev) / 1000000.0;
	timerPrev = nowTime;
}




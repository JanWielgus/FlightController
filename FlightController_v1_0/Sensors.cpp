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
	
	#ifdef FILTR_KALMANA
		fkPitch.init(0.001, 0.003, 0.03);
		fkRoll.init(0.001, 0.003, 0.03);
	#endif
}



void SensorsClass::readAngles()
{
	#ifdef FILTR_KALMANA
		//dt_ = (double)(micros() - timerPrev) / 1000000;  // dla PID
		Vector accVal = mpu.readNormalizeAccel();
		Vector gyrVal = mpu.readNormalizeGyro();
		
		pitchAcc = -(atan2(accVal.XAxis, sqrt(accVal.YAxis*accVal.YAxis + accVal.ZAxis*accVal.ZAxis))*180.0)/M_PI;
		rollAcc  =  (atan2(accVal.YAxis, accVal.ZAxis)*180.0)/M_PI;
		
		angle.pitch = fkPitch.update(pitchAcc, gyrVal.YAxis);
		angle.roll  = fkRoll.update(rollAcc, gyrVal.XAxis);
		
		//timerPrev = micros();  // dla PID
		
	#else
		//dt_ = (double)(micros() - timerPrev) / 1000000;
		
		// Read normalized values
		Vector norm = mpu.readNormalizeGyro();
		Vector normAccel = mpu.readNormalizeAccel();
		
		// Calculate Pitch, Roll and Yaw (Gyro)
		angle.pitch += (norm.YAxis-GYRO_X_OFFSET) * dt_;
		angle.roll  += (norm.XAxis-GYRO_Y_OFFSET) * dt_;
		angle.yaw   += (norm.ZAxis-GYRO_Z_OFFSET) * dt_;

		// Calculate Pitch and Roll (Acc)
		pitchAcc = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
		rollAcc =   (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
		
		// Offsets
		pitchAcc -= PITCH_OFFSET;
		rollAcc  -= ROLL_OFFSET;
		
		//-- Filtr komplementarny --
		angle.pitch = 0.98*angle.pitch + 0.02*pitchAcc;
		angle.roll  = 0.98*angle.roll  + 0.02*rollAcc;
		//timerPrev = micros();
	#endif
}




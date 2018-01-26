// Sensors.h
// Author: Jan Wielgus
// Date: 25.10.2017

#ifndef _SENSORS_h
#define _SENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <I2Cdev.h>
//#include <MPU6050_6Axis_MotionApps20.h>
#include "C:\Users\Janek\Documents\GitHub\FlightController\FlightController_v1_0\MPU6050.h"
#include <Wire.h>

//#include <MS5611.h>
#include "config.h"


class SensorsClass
{
 public:
	MPU6050 mpu;
// === FUNKCJE ===
 public:
	void init();
	void readAngles(); // roll, pitch, yaw
	
// === ZMIENNE ===
 public:
	//double pitch = 0;
	//double roll = 0;
	//float  yaw = 0;
	struct ypr_angles
	{
		double pitch = 0;
		double roll = 0;
		float  yaw = 0;
		}angle;
	
	volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
	
 private:
	// MPU control/status vars
	bool dmpReady = false;  // set true if DMP init was successful
	uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
	uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer
	
	// orientation/motion vars
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
};

extern SensorsClass sensors;

#endif


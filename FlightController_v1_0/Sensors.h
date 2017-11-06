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

#include <Wire.h>
#include <MPU6050.h>
#include <MS5611.h>


class SensorsClass
{
 public:
	MPU6050 mpu;
// === FUNKCJE ===
 public:
	void init();
	void readAngles(); // roll, pitch, yaw
	//void updateDeltaTime(); // Zwraca czas wykonywania programu (czas od poprzedniego wywo³ania)
	
// === ZMIENNE ===
 public:
	double pitch = 0;
	double roll = 0;
	float yaw = 0;
	double dt_;
	
 private:
	float pitchAcc = 0;
	float rollAcc = 0;
	uint32_t timerPrev = 0;
};

extern SensorsClass sensors;

#endif


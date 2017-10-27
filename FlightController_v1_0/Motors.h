// Motors.h
// Author: Jan Wielgus
// Date: 25.10.2017

#ifndef _MOTORS_h
#define _MOTORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Servo.h>
#include "config.h"


class MotorsClass
{
 public:
	Servo mTL;
	Servo mTR;
	Servo mBL;
	Servo mBR;
	
// === FUNKCJE ===
 public:
	void init(); // wskaünik na obiekt serwa i pin silnika
	void setOnAllMotors(uint16_t _val); // parametr od 0 do 1000
	void setOnEachMotor(uint16_t mot1, uint16_t mot2, uint16_t mot3, uint16_t mot4);
	
// === ZMIENNE ===
 public:
	
};

extern MotorsClass motors;

#endif


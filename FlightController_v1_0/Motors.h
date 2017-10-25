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



class MotorsClass
{
// === FUNKCJE ===
 public:
	void init(Servo *_servo, int _pin); // wskaünik na obiekt serwa i pin silnika
	void setMotor(uint16_t _val); // parametr od 0 do 2000
	
// === ZMIENNE ===
 public:
	int16_t main_power; // g≥Ûwna moc (nie wiem czy potrzebna)
	Servo *motor; // wskaünik do sterowania silnikiem (serwo)
};

//extern MotorsClass motors;

#endif


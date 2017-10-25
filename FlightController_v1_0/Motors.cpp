// Author: Jan Wielgus
// Date: 25.10.2017
// 

#include "Motors.h"
//MotorsClass motors;



void MotorsClass::init(Servo *_servo, int _pin)
{
	motor = _servo; // przekazanie wska�nika na wska�nik globalny
	
	motor->attach(_pin); // uruchomienie serwa na pinie
}



void MotorsClass::setMotor(uint16_t _val)
{
	// Zostawi� to pierwsze (i przetestowa�)
	/*
	_val = map(_val, 0, 2000, 1000, 2000);
	_val = constrain(_val, 1000, 2000);
	motor->writeMicroseconds(_val);
	*/
	
	_val = map(_val, 0, 2000, 0, 180);
	_val - constrain(_val, 0, 180);
	motor->write(_val);
}




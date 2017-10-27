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

#define TLmotorPin 5
#define TRmotorPin 6
#define BLmotorPin 9
#define BRmotorPin 10



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
	void setOnAllMotors(uint16_t _val); // parametr od 0 do 2000
	
// === ZMIENNE ===
 public:
	//int16_t main_power; // g≥Ûwna moc (nie wiem czy potrzebna)
};

extern MotorsClass motors;

#endif


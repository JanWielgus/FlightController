// Author: Jan Wielgus
// Date: 25.10.2017
// 

#include "Motors.h"
MotorsClass motors;



void MotorsClass::init()
{
	mTL.attach(TLmotorPin); // Top left
	mTR.attach(TRmotorPin); // Top right
	mBL.attach(BLmotorPin); // Back left
	mBR.attach(BRmotorPin); // Back right
}



void MotorsClass::setOnAllMotors(uint16_t _val)
{
	_val += 1000;
	_val = constrain(_val, 1000, 2000);
	
	mTL.writeMicroseconds(_val);
	mTR.writeMicroseconds(_val);
	mBL.writeMicroseconds(_val);
	mBR.writeMicroseconds(_val);
}



void MotorsClass::setOnEachMotor(uint16_t mot1, uint16_t mot2, uint16_t mot3, uint16_t mot4)
{
	//mot1 = (mot1 / 2) + 1000;
}



